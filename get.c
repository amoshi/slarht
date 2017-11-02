#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <dirent.h>
#include "evhttp.h"
#include "mkdirp.h"
#define HT_FOOT "</pre><hr></body></html>\n"

uint64_t get_headers(struct evhttp_request *req, http_traf *ht)
{
	uint64_t i;
	struct evkeyvalq *headers;
	struct evkeyval *header;
	http_kv *ht_headers;
	headers = evhttp_request_get_input_headers(req);
	for (i=0, header = headers->tqh_first; header; header = header->next.tqe_next, i++)
	{
		printf("  %s: %s\n", header->key, header->value);
	}
	ht->headers_len=i;
	ht->headers = malloc (sizeof (http_kv)*ht->headers_len );
	for (i=0, header = headers->tqh_first; header; header = header->next.tqe_next, i++)
	{
		ht->headers[i].key = header->key;
		ht->headers[i].value = header->value;
		if ( !strcasecmp(ht->headers[i].key, "Host") )
			ht->host=header->value, ht->host_size=strlen( header->value );
		if ( !strcasecmp(ht->headers[i].key, "X-slarht-template") )
			ht->_template=header->value, ht->host_size=strlen( header->value );
		if ( !strcasecmp(ht->headers[i].key, "X-architecture") )
			ht->pkg_architecture=header->value, ht->pkg_architecture_size=strlen( header->value );
		if ( !strcasecmp(ht->headers[i].key, "X-distribution") )
			ht->pkg_distribution=header->value, ht->pkg_distribution_size=strlen( header->value );
		if ( !strcasecmp(ht->headers[i].key, "Content-Type") )
			ht->content_type=header->value, ht->content_type_size=strlen( header->value );
	}
	return ht->headers_len;
}
get_args(http_traf *ht)
{
	struct evkeyvalq *http_args;
	http_args = malloc (sizeof(struct evkeyval)*50);
	char *ptrtoargs = strstr(ht->full_uri,"?");
	printf("%s, \n",ht->full_uri);
	if ( ptrtoargs != NULL )
	{
		ht->query = alloc_and_copy_n(ht->full_uri, (size_t)(ptrtoargs-ht->full_uri) );
		printf("size=%d\n",ptrtoargs-ht->full_uri);
		printf ("put into parser: %s\n", ptrtoargs+1);
		return evhttp_parse_query_str(ptrtoargs+1, http_args);
	}
	else
	{
		ht->query = alloc_and_copy_n(ht->full_uri, ht->full_uri_size );
		return -1;
	}
}

uint64_t get_file_size(FILE *fd)
{
	struct stat st;
	if (fstat(fd, &st) < 0)
	{
		perror("fstat");
		return 0;
	}
	return st.st_size;
}

char* gen_directory_index(char *fpath, char *repo_uri, char *repo)
{
//<html><head><title>Index of /repo/mephi/evm/</title></head><body bgcolor="white"><h1>Index of /repo/mephi/evm/</h1><hr><pre>
//<a href="../">../</a>
//<a href="2th">2th</a>                                                20-Oct-2017 15:50               21685
//<a href="3rth">3rth</a>                                               27-Oct-2017 06:16               28033
//<a href="less1.txt">less1.txt</a>                                          27-Oct-2017 06:16                3958
//</pre><hr></body></html>
	DIR *dir;
	struct dirent *entry;
	dir = opendir(fpath);
	char *dirindex;
	if (!dir)
	{
		printf("%s: ", fpath);
		perror("opendir");
	}
	else
	{
		size_t obj_nums = 0;
		size_t obj_lens = 0;
		size_t ht_foot_size = strlen(HT_FOOT);
		uint64_t curs = 0;
		char ht_head[1000];
		snprintf(ht_head, 1000, "<html><head><title>Index of %s</title></head><body bgcolor=\"white\"><h1>Index of %s</h1><hr><pre>\n", repo, repo);
		size_t ht_head_size = strlen(ht_head);
		while ( (entry = readdir(dir)) != NULL)
		{
			if ( entry->d_name[0]=='.' && entry->d_name[1]=='.' )
				continue;
			obj_nums++;
			obj_lens += (strlen(entry->d_name)*2);
			obj_lens += strlen(fpath);
			obj_lens += strlen(repo_uri);
			printf(" +obj_lens = %zu (%s)\n", (strlen(entry->d_name)*2), entry->d_name );
		}
		printf(" ht_head_size = %zu\n", ht_head_size);
		printf(" ht_foot_size = %zu\n", ht_foot_size);
		printf(" obj_nums*17 = %zu\n", obj_nums*17);
		obj_lens += ht_head_size;
		obj_lens += ht_foot_size;
		obj_lens += obj_nums*17;

		dirindex = malloc(obj_lens);
		curs = ht_head_size ;
		strncpy(dirindex, ht_head, curs);
		rewinddir(dir);
		printf("len=%zu\n", obj_lens);
		while ( (entry = readdir(dir)) != NULL)
		{
			printf("name=%s (%zu)\n",entry->d_name, strlen(entry->d_name));
			if ( entry->d_name[0]=='.' && entry->d_name[1]=='.' )
				continue;
			if ( entry->d_name[0]=='.' && strlen(entry->d_name)==1 )
				continue;
			printf ("size=%zu\n",curs + ht_foot_size + 17 + strlen( entry->d_name )*2);
			printf ("%"PRIu64" + %zu + %d + strlen(%s)*2{%zu} > %zu\n", curs, ht_foot_size, 17, entry->d_name, strlen( entry->d_name )*2, obj_lens);
			if (curs + ht_foot_size + 17 + strlen( entry->d_name )*2 > obj_lens )
				break;
			printf(" adding %s to index\n", entry->d_name);
			printf("pre curs=%zu; index=%s\n", curs, dirindex);
			curs += sprintf(dirindex+curs, "<a href=\"%s/%s\">%s</a>\n", repo_uri, entry->d_name, entry->d_name);
			printf("post curs=%zu; index=%s\n", curs, dirindex);
		}
		strncat(dirindex, HT_FOOT, ht_foot_size);
		closedir(dir);
	}
	return dirindex;
}

int do_Get(http_traf *ht, slarht_conf_general *sc_general, struct evhttp_request *req, struct evbuffer *returnbuffer)
{
	int httpcode = 0;
	get_headers(req, ht);
	if ( get_args(ht) != 0 )
	{
		printf("args parsing error\n");
		//return 400;
	}
	puts("1");

	if ( route_resolver(sc_general, ht) == -1 )
	{
		puts("no defaults, stopping query");
		//return;
	}
	puts("2");

	printf("repo: %s\n", ht->sc_repository->name);


	uint64_t filepath_size = strlen(ht->query+ht->sc_repository->uri_size);
	ht->filepath_size = filepath_size;
	ht->filepath = copy_init_n(ht->query+ht->sc_repository->uri_size, filepath_size+1);
	ht->filename = basename(ht->filepath,DIRBASENAME_B,filepath_size);
	ht->filename_size = strlen(ht->filename);
	if ( ht->filename_size == 0 )
	{
		snprintf(ht->filename,2,"%s","t");
		ht->filename_size=2;
	}
	ht->dirname = basename(ht->filepath,DIRBASENAME_D,filepath_size);
	ht->dirname_size = strlen(ht->dirname);
	printf("filepath = %s{%d}\n",ht->filepath,ht->filepath_size);
	printf("filename = %s{%d}\n",ht->filename,ht->filename_size);
	printf("dirname = %s{%d}\n",ht->dirname,ht->dirname_size);
	puts("finish matching");



	char fpath[FILENAME_MAX];
	snprintf(fpath, FILENAME_MAX, "%s/%s", ht->sc_repository->filesystem, ht->filepath);
	int ftype = filetype(fpath);

	if ( ftype == ISTAT_FILE )
	{
		httpcode=HTTP_OK;
		int fpath_fd = open(fpath, O_RDONLY);
		if ( fpath_fd == 0 )
		{
			perror("open()");
		}
		else
		{
			struct stat st;
			if (fstat(fpath_fd, &st)<0)
			{
				perror("fstat");
			}
			evbuffer_add_file(returnbuffer, fpath_fd, 0, st.st_size);
		}
	}
	else if ( ftype == ISTAT_DIRECTORY )
	{
		httpcode=HTTP_OK;
		char *dindex = gen_directory_index(fpath, ht->query, ht->filepath);
		evbuffer_add_printf(returnbuffer, dindex);
		free(dindex);
	}
	else if ( ftype == ISTAT_NOTFOUND )
	{
		httpcode=HTTP_NOTFOUND;
		evbuffer_add_printf(returnbuffer, "{\n \"error\": \"file not found\",\n \"code\": %d\n}\n", httpcode);
	}
	else
	{
		httpcode=403;
		evbuffer_add_printf(returnbuffer, "{\n \"error\": \"no access to this function or file\",\n \"code\": %d\n}\n", httpcode);
	}


	//printf("{\n \"repo\": \"%s\",\n \"path\": \"%s\",\n \"created\": \"%s\",\n \"createdBy\": \"%s\",\n \"downloadUri\": \"%s\",\n \"size\": %zu,\n \"checksums\":\n {\n  \"sha1\": \"%s\",\n  \"md5\":\"%s\"\n }\n}\n",ht->sc_repository->name, ht->filepath, "no", "no", ht->downloaduri, ht->data_size, "no","no");
	puts("7");
	puts("5");
	evhttp_send_reply(req, httpcode, "Client", returnbuffer);
	puts("4");


	printf("filesystem is %s\n", ht->sc_repository->filesystem);
	printf("fulluri is %s\n", ht->query);
	printf("filepath is %s\n", fpath);





	evbuffer_free(returnbuffer);
	//free(ht->downloaduri);
	puts("exit get");
}
