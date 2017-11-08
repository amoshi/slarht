#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <dirent.h>
#include <string.h>
#include "evhttp.h"
#include "mkdirp.h"
#include "get.h"
#include "strtls.h"
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
		if ( !strcasecmp(ht->headers[i].key, "X-Auth-Token") )
			ht->authtoken=header->value, ht->authtoken_size=strlen( header->value );
		if ( !strcasecmp(ht->headers[i].key, "Authorization") )
			get_http_auth(header->value, ht);
	}
	return ht->headers_len;
}
int get_args(http_traf *ht)
{
	puts("start parsing args");
	struct evkeyvalq *http_args;
	struct evkeyval *http_arg;
	http_args = malloc (sizeof(struct evkeyval)*50);
	char *ptrtoargs = strstr(ht->full_uri,"?");
	uint64_t i;
	printf("%s, \n",ht->full_uri);
	if ( ptrtoargs != NULL )
	{
		ht->query = copy_init_n(ht->full_uri, (size_t)(ptrtoargs-ht->full_uri) );
		printf("size=%d\n",ptrtoargs-ht->full_uri);
		printf ("Args: %s\n", ptrtoargs+1);
		evhttp_parse_query_str(ptrtoargs+1, http_args);
		for (i=0, http_arg = http_args->tqh_first; http_arg; http_arg = http_arg->next.tqe_next, i++);
		//{
		//	printf("  %s: %s\n", http_arg->key, http_arg->value);
		//}
		ht->args_len = i;
		ht->args = malloc(sizeof(http_kv)*ht->args_len);
		for (i=0, http_arg = http_args->tqh_first; http_arg; http_arg = http_arg->next.tqe_next, i++)
		{
			ht->args[i].key = http_arg->key;
			ht->args[i].value = http_arg->value;
			printf("  %s: %s\n", http_arg->key, http_arg->value);
		}
		return 0;
	}
	else
	{
		ht->query = copy_init_n(ht->full_uri, ht->full_uri_size );
		ht->args_len = 0;
		ht->args = NULL;
		return -1;
	}
}




char* gen_directory_index(void *fpath, void *repo_uri, void *repo)
{
	DIR *dir;
	struct dirent *entry;
	dir = opendir(fpath);
	char *dirindex = NULL;
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
	}

	if ( route_resolver(sc_general, ht) == -1 )
	{
		puts("no defaults, stopping query");
	}

	printf("repo: %s\n", ht->sc_repository->name);
	repo_init(ht);

	int auth = check_auth(ht);
	printf("!!AUTH!! %d\n", auth);
	if ( auth < ht->sc_repository->access_level )
	{
		int httpcode=403;
		evbuffer_add_printf(returnbuffer, "{\n \"error\": \"no access for writing\",\n \"code\": %d\n}\n", httpcode);
		evhttp_send_reply(req, httpcode, "Client", returnbuffer);
		evbuffer_free(returnbuffer);
		return httpcode;
	}


	uint64_t filepath_size = strlen(ht->query+ht->sc_repository->uri_size);
	ht->filepath_size = filepath_size;
	ht->filepath = copy_init_n(ht->query+ht->sc_repository->uri_size, filepath_size+1);
	printf("ht->filepath %s\n", ht->filepath);
	printf("filepath_size %"PRIu64"\n", filepath_size);
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

	int no_category = 1;
	if ( ftype == ISTAT_DIRECTORY )
	{
		no_category = 0;
		int dir=1;
		printf("dirtoindex=%d\n", ht->dir_to_index);
		if ( ht->dir_to_index == 1 )
		{
			char indexfile[FILENAME_MAX];
			snprintf(indexfile, FILENAME_MAX, "%s/%s", fpath, INDEXFILE);
			printf("check for index file: %s\n", indexfile);
			ftype = filetype(indexfile);
			printf("ftype=%d\n", ftype);
			if ( ftype == ISTAT_FILE )
			{
				printf("follow to index.html\n");
				strncat(fpath, INDEXFILE, strlen(INDEXFILE)+1);
				dir=0;
			}
		}
		if ( dir == 1 )
		{
			httpcode=HTTP_OK;
			//char *dindex = gen_directory_index(fpath, ht->query, ht->filepath);
			char *dindex = (ht->index_generator)(fpath, ht->query, ht->filepath);
			evbuffer_add_printf(returnbuffer, dindex);
			free(dindex);
		}
	}
	if ( ftype == ISTAT_FILE )
	{
		no_category = 0;
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
	if ( ftype == ISTAT_NOTFOUND )
	{
		no_category = 0;
		httpcode=HTTP_NOTFOUND;
		evbuffer_add_printf(returnbuffer, "{\n \"error\": \"file not found\",\n \"code\": %d\n}\n", httpcode);
	}
	if ( no_category == 1 )
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
