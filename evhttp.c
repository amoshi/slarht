#include <sys/types.h>
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <inttypes.h>
 
#include <event.h>
#include <evhttp.h>
#include <time.h>

#include "evhttp.h"
#include "config_yaml.h"
#include "mkdirp.h"
#include "post.h"
#include "rpm.h"
#include "generic.h"
#include "deb.h"
#include "pypi.h"
#include "strtls.h"
//#include "rpmburner.h"

http_traf *http_traf_initialize(uint64_t size)
{
	http_traf *ht;
	ht->method_id=0;
	ht->method_size=0;
	ht->query = malloc(HTTP_QUERY_MAXSIZE);
	ht->query_size = 0;
	ht->data = malloc(HTTP_QUERY_MAXSIZE);
	ht->data_size=0;
	ht->args = malloc(sizeof(http_kv)*HTTP_ARGS_MAXCOUNT);
	ht->headers = malloc(sizeof(http_kv)*HTTP_ARGS_MAXCOUNT);
	ht->headers_len = 0;
	ht->args_len = 0;

	return ht;
}


void generic_request_handler(struct evhttp_request *req, void *arg)
{
	struct evbuffer *returnbuffer = evbuffer_new();
	struct evkeyvalq *headers, *http_args;
	struct evkeyval *header, *http_arg;
	struct evbuffer *buf;
	slarht_conf_general *sc_general = arg;

	mkdirp(sc_general->tmpdir);

	//int no_http_args = 0;
	http_traf *ht = malloc(sizeof(http_traf));
	ht->full_uri = evhttp_request_get_uri(req);
	ht->full_uri_size = strlen(ht->full_uri);
	ht->authtoken_size = 0;
	ht->http_basic_hash_size = 0;
	uint64_t http_uri_size = ht->full_uri_size;
	char *http_uri = ht->full_uri;
	http_args = malloc (sizeof(struct evkeyval)*50);
	char *data;
	char *file_cache_path;
	uint64_t i, file_cache = 0;
	uint64_t files_count = 1;
	char *nodata = copy_init("nodata");;
	char *emptydata = copy_init("");;
	ht->host = nodata;
	ht->_template = nodata;
	ht->pkg_architecture = NULL;
	ht->pkg_distribution = NULL;
	ht->host_size = 7;
	ht->_template_size = 7;
	ht->pkg_architecture_size = 0;
	ht->pkg_distribution_size = 0;
	ht->prefix_repository_url = emptydata;
	postdata *pstdat;

	ht->method_id=evhttp_request_get_command(req);
	int querycodetype=ht->method_id;
	int method_id=ht->method_id;
	switch (method_id) {
		case EVHTTP_REQ_GET: ht->method = copy_init_n("GET", 3); ht->method_size=3; do_Get(ht, sc_general, req, returnbuffer); return;
		case EVHTTP_REQ_POST: ht->method = copy_init_n("POST", 4); ht->method_size=4; break;
		case EVHTTP_REQ_HEAD: ht->method = copy_init_n("HEAD", 4); ht->method_size=4; break;
		case EVHTTP_REQ_PUT: ht->method = copy_init_n("PUT", 3); ht->method_size=3; break;
		case EVHTTP_REQ_DELETE: ht->method = copy_init_n("DELETE", 6); ht->method_size=6; break;
//		case EVHTTP_REQ_TRACE: ht->method = copy_init_n("TRACE", 5); ht->method_size=5; break;
//		case EVHTTP_REQ_PATCH: ht->method = copy_init_n("PATCH", 5); ht->method_size=5; break;
//		case EVHTTP_REQ_OPTIONS: ht->method = copy_init_n("OPTIONS", 7); ht->method_size=7; break;
//		case EVHTTP_REQ_CONNECT: ht->method = copy_init_n("CONNECT", 7); ht->method_size=7; break;
		default:
			evbuffer_add_printf(returnbuffer, "Method not allowed!\n");
			evhttp_send_reply(req, HTTP_BADMETHOD, "Client", returnbuffer);
			evbuffer_free(returnbuffer);
			return;
			break;
	}

	printf("enum=%d\n",evhttp_request_get_command(req));
 
	get_headers(req, ht);

 
	buf = evhttp_request_get_input_buffer(req);

	if ( querycodetype == EVHTTP_REQ_GET || querycodetype == EVHTTP_REQ_HEAD || querycodetype == EVHTTP_REQ_DELETE || querycodetype == EVHTTP_REQ_PUT )
	{
		if ( get_args(ht) != 0 )
		{
			printf("args parsing error\n");
		}
	}
	else
	{
		ht->query = http_uri;
		ht->query_size = http_uri_size;
	}
	if ( route_resolver(sc_general, ht) == -1 )
	{
		puts("no defaults, stopping query");
		evbuffer_add_printf(returnbuffer, "ERROR, no routes!\n");
		evhttp_send_reply(req, HTTP_NOTIMPLEMENTED, "Client", returnbuffer);
		evbuffer_free(returnbuffer);
		return;
	}
	printf("repo: %s\n", ht->sc_repository->name);

	int auth = check_auth(ht);
	printf("!!AUTH!! %d\n", auth);
	if ( auth != 2 )
	{
		int httpcode=403;
		evbuffer_add_printf(returnbuffer, "{\n \"error\": \"no access for writing\",\n \"code\": %d\n}\n", httpcode);
		evhttp_send_reply(req, httpcode, "Client", returnbuffer);
		evbuffer_free(returnbuffer);
		return;
	}


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
	if ( ht->sc_repository->type_id == REPOSITORY_TYPE_APT )
		file_cache = 1;
	if ( querycodetype == EVHTTP_REQ_POST )
		file_cache = 1;

	if ( querycodetype == EVHTTP_REQ_POST || querycodetype == EVHTTP_REQ_PUT )
	{
		size_t len = evbuffer_get_length(buf);
		ht->data = malloc ( len );
		ht->data_size = len;
		data = ht->data;
		data[0]='\0';
		FILE *fbuf;
		if ( file_cache == 1 )
		{
			file_cache_path = malloc(UCHAR_MAX);
			char file_cache_dir[UCHAR_MAX];
			struct timespec now_time;
			clock_gettime(CLOCK_REALTIME, &now_time);
			snprintf(file_cache_dir,UCHAR_MAX-1,"%s/%lld.%09ld/%s",sc_general->tmpdir,now_time.tv_sec,now_time.tv_nsec,ht->dirname);
			snprintf(file_cache_path,UCHAR_MAX-1,"%s/%s",file_cache_dir,ht->filename);
			mkdirp(file_cache_dir);
			fprintf(stdout, "file_cache_path (%p) is %s\n", file_cache_path, file_cache_path);
			fprintf(stdout, "file_cache_dir (%p) is %s\n", file_cache_dir, file_cache_dir);
			
			if ( ( fbuf = fopen (file_cache_path, "w") ) == NULL )
			{	return;
			}
			ht->file_cache_path = file_cache_path;
		}
		char *ptr_to_memcpy = ht->data;
		while (evbuffer_get_length(buf))
		{
			int n;
			char cbuf[1024];
			n = evbuffer_remove(buf, cbuf, 1023);
			if (n > 0 && file_cache == 1)
				(void) fwrite(cbuf, 1, n, fbuf);
			memcpy(ptr_to_memcpy, cbuf, n);
			ptr_to_memcpy += n;
		}
		if ( file_cache == 1 )
		{
			fclose(fbuf);
		}
	}
	if (  querycodetype == EVHTTP_REQ_POST)
	{
		if ( startswith_matches_n(ht->content_type, "multipart/form-data", 19) == 19 )
		{
			printf("post multipart/form-data\n");
			char *boundary = strstr(ht->content_type,POST_MULTIPART_BOUNDARY)+POST_MULTIPART_BOUNDARY_SIZE+1;
			char *filename = gen_tmp_filename (sc_general->tmpdir, "http", "post");
			mkdirp(filename);
			pstdat = post_multipart_parse(file_cache_path, POST_MULTI_DATA_FROM_FILE, POST_MULTI_DATA_FROM_FILE, boundary, strlen(boundary), filename);

			//int i;
			//printf("kv_size=%"PRId64"\n", pstdat->kv_size);
			//for ( i=0; i<pstdat->kv_size;
			//	printf("kv[%d].key=%s(%zu)\n", i, pstdat->kv[i].key, pstdat->kv[i].key_size),
			//	printf("kv[%d].value=%s(%zu)\n", i, pstdat->kv[i].value, pstdat->kv[i].value_size),
			//	i++
			//);
			//printf("fkv_size=%"PRId64"\n", pstdat->fkv_size);
			//for ( i=0; i<pstdat->fkv_size;
			//	printf("fkv[%d].key=%s(%zu)\n", i, pstdat->fkv[i].key, pstdat->fkv[i].key_size),
			//	printf("fkv[%d].value=%s(%zu)\n", i, pstdat->fkv[i].value, pstdat->fkv[i].value_size),
			//	i++
			//);
			ht->args_len = pstdat->kv_size;
			ht->args = pstdat->kv;
			ht->file_cache_path = pstdat->fkv->value;
			files_count = pstdat->fkv_size;
			printf("files_count is %"PRIu64"\n",files_count);
		}
		else
		{
			printf ("put into parser: %s\n", data);
			evhttp_parse_query_str(data, http_args);
		}
	}

	// deploy and answers
	// for POST and PUT (uploading artifact)
	if ( querycodetype == EVHTTP_REQ_POST || querycodetype == EVHTTP_REQ_PUT )
	{
		printf("files_count is %zu\n",files_count);
		for ( i = 0; i < files_count; i++ )
		{
			if (  querycodetype == EVHTTP_REQ_POST)
			{
				ht->file_cache_path = pstdat->fkv[i].value;
				filepath_size = strlen(pstdat->fkv[i].value);
				ht->filepath_size = filepath_size;
				ht->filepath = copy_init_n(pstdat->fkv[i].value, filepath_size+1);
				ht->filename = basename(ht->filepath,DIRBASENAME_B,filepath_size);
				ht->filename_size = strlen(ht->filename);
				if ( ht->filename_size == 0 )
				{
					snprintf(ht->filename,2,"%s","t");
					ht->filename_size=2;
				}
				ht->dirname = basename(ht->filepath,DIRBASENAME_D,filepath_size);
				ht->dirname_size = strlen(ht->dirname);
			}


			repo_conf *rconf = NULL;
			if ( ht->sc_repository->type_id == REPOSITORY_TYPE_YUM )
				rconf = rpm_conf(ht);
			else if ( ht->sc_repository->type_id == REPOSITORY_TYPE_GENERIC )
				rconf = generic_conf(ht);
			else if ( ht->sc_repository->type_id == REPOSITORY_TYPE_APT )
				rconf = deb_conf(ht);
			else if ( ht->sc_repository->type_id == REPOSITORY_TYPE_DOCKER )
				rconf = generic_conf(ht);
			else if ( ht->sc_repository->type_id == REPOSITORY_TYPE_PYPI )
				rconf = pypi_conf(ht, pstdat);
			ht->downloaduri = malloc(UCHAR_MAX);
			snprintf(ht->downloaduri,UCHAR_MAX-1,"http://%s/%s/%s/%s",ht->host,ht->sc_repository->uri,ht->prefix_repository_url,ht->filepath);
			if ( rconf != NULL )
			{
				repoburner(rconf);
			}
			else
			{
				evbuffer_add_printf(returnbuffer, "{\n \"error\": \"%s\" }\n",ht->error_message);
				evhttp_send_reply(req, HTTP_OK, "Client", returnbuffer);
				evbuffer_free(returnbuffer);
				return;
			}
			if ( querycodetype == EVHTTP_REQ_PUT || querycodetype == EVHTTP_REQ_POST )
				free(data);

			if ( file_cache == 1 )
			{
				free(file_cache_path);
			}

			puts("");
			printf("{\n \"repo\": \"%s\",\n \"path\": \"%s\",\n \"created\": \"%s\",\n \"createdBy\": \"%s\",\n \"downloadUri\": \"%s\",\n \"size\": %zu,\n \"checksums\":\n {\n  \"sha1\": \"%s\",\n  \"md5\":\"%s\"\n }\n}\n",ht->sc_repository->name, ht->filepath, "no", "no", ht->downloaduri, ht->data_size, "no","no");
			evbuffer_add_printf(returnbuffer, "{\n \"repo\": \"%s\",\n \"path\": \"%s\",\n \"created\": \"%s\",\n \"createdBy\": \"%s\",\n \"downloadUri\": \"%s\",\n \"size\": %zu,\n \"checksums\":\n {\n  \"sha1\": \"%s\",\n  \"md5\":\"%s\"\n }\n}\n",ht->sc_repository->name, ht->filepath, "no", "no", ht->downloaduri, ht->data_size, "no","no");
			evhttp_send_reply(req, HTTP_OK, "Client", returnbuffer);
			evbuffer_free(returnbuffer);
			free(ht->downloaduri);
			if ( rconf->deploy_method=DEPLOY_METHOD_SCRIPT)
			{
				free(rconf->command);
			}
			free(rconf);
		}
	}
	else if ( querycodetype == EVHTTP_REQ_GET )
	{
		// for GET query
			evhttp_send_reply(req, HTTP_OK, "Client", returnbuffer);
			evbuffer_free(returnbuffer);
		
	}
	return;
}

int main(int argc, char **argv)
{
	char *config_path = argc ? argv[1] : "/etc/slarht/slarht.yaml";
	slarht_conf_general *sc_general = get_config(config_path);
	repcheck(sc_general);
	int64_t	http_port = sc_general->port;
	char	*http_addr = sc_general->listen;
	struct	evhttp *http_server = NULL;
 
	event_init();
	http_server = evhttp_start(http_addr, http_port);
	evhttp_set_gencb(http_server, generic_request_handler, (void*)sc_general);
 
	fprintf(stderr, "Server started on port %u\n", http_port);
	event_dispatch();
 
	return(0);
}
