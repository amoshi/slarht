#include <sys/types.h>
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
 
#include <event.h>
#include <evhttp.h>
#include <time.h>

#include "evhttp.h"
#include "config_yaml.h"
#include "mkdirp.h"
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

char *alloc_and_copy_n(char *data, size_t len)
{
	len++;
	char *s = malloc(len);
	snprintf(s,len,"%s",data);
	return s;
}

void generic_request_handler(struct evhttp_request *req, void *arg)
{
	struct evbuffer *returnbuffer = evbuffer_new();
	struct evkeyvalq *headers, *http_args;
	struct evkeyval *header, *http_arg;
	struct evbuffer *buf;
	slarht_conf_general *sc_general = arg;

	mkdir(sc_general->tmpdir);

	int no_http_args = 0;
	http_traf *ht = malloc(sizeof(http_traf));
	ht->full_uri = evhttp_request_get_uri(req);
	ht->full_uri_size = strlen(ht->full_uri);
	uint64_t http_uri_size = ht->full_uri_size;
	char *http_uri = ht->full_uri;
	http_args = malloc (sizeof(struct evkeyval)*50);
	char *data;
	char *file_cache_path;
	uint64_t i, file_cache = 0;
	char *nodata = copy_init("nodata");;
	ht->host = nodata;
	ht->host_size = 7;
	ht->template = nodata;
	ht->template_size = 7;

	ht->method_id=evhttp_request_get_command(req);
	int querycodetype=ht->method_id;
	int method_id=ht->method_id;
	switch (method_id) {
		case EVHTTP_REQ_GET: ht->method = alloc_and_copy_n("GET", 3); ht->method_size=3; break;
		case EVHTTP_REQ_POST: ht->method = alloc_and_copy_n("POST", 4); ht->method_size=4; break;
		case EVHTTP_REQ_HEAD: ht->method = alloc_and_copy_n("HEAD", 4); ht->method_size=4; break;
		case EVHTTP_REQ_PUT: ht->method = alloc_and_copy_n("PUT", 3); ht->method_size=3; break;
		case EVHTTP_REQ_DELETE: ht->method = alloc_and_copy_n("DELETE", 6); ht->method_size=6; break;
//		case EVHTTP_REQ_TRACE: ht->method = alloc_and_copy_n("TRACE", 5); ht->method_size=5; break;
//		case EVHTTP_REQ_PATCH: ht->method = alloc_and_copy_n("PATCH", 5); ht->method_size=5; break;
//		case EVHTTP_REQ_OPTIONS: ht->method = alloc_and_copy_n("OPTIONS", 7); ht->method_size=7; break;
//		case EVHTTP_REQ_CONNECT: ht->method = alloc_and_copy_n("CONNECT", 7); ht->method_size=7; break;
		default:
			evbuffer_add_printf(returnbuffer, "Method not allowed!\n");
			evhttp_send_reply(req, HTTP_BADMETHOD, "Client", returnbuffer);
			evbuffer_free(returnbuffer);
			return;
			break;
	}

	printf("enum=%d\n",evhttp_request_get_command(req));
 
	printf("Received a %d request for %s\nHeaders:\n", ht->method, http_uri);
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
			ht->host=header->value, ht->host_size=strlen( ht->host );
		if ( !strcasecmp(ht->headers[i].key, "X-slarht-template") )
			ht->template=header->value, ht->host_size=strlen( ht->template );
	}
 
	buf = evhttp_request_get_input_buffer(req);

	if ( querycodetype == EVHTTP_REQ_GET || querycodetype == EVHTTP_REQ_HEAD || querycodetype == EVHTTP_REQ_DELETE || querycodetype == EVHTTP_REQ_PUT )
	{
		char *ptrtoargs = strstr(http_uri,"?");
		printf("%s, \n",http_uri);
		if ( ptrtoargs != NULL )
		{
			ht->query = alloc_and_copy_n(http_uri, (size_t)(ptrtoargs-http_uri) );
			printf("size=%d\n",ptrtoargs-http_uri);
			printf ("put into parser: %s\n", ptrtoargs+1);
			evhttp_parse_query_str(ptrtoargs+1, http_args);
		}
		else
		{
			ht->query = alloc_and_copy_n(http_uri, http_uri_size );
			no_http_args = 1;
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
	uint64_t filepath_size = strlen(ht->query+ht->sc_repository->uri_size);
	ht->filepath_size = filepath_size;
	ht->filepath = copy_init_n(ht->query+ht->sc_repository->uri_size, filepath_size+1);
	ht->filename = basename(ht->filepath,DIRBASENAME_B,filepath_size);
	ht->filename_size = strlen(ht->filename);
	ht->dirname = basename(ht->filepath,DIRBASENAME_D,filepath_size);
	ht->dirname_size = strlen(ht->dirname);
	printf("filepath = %s{%d}\n",ht->filepath,ht->filepath_size);
	printf("filename = %s{%d}\n",ht->filename,ht->filename_size);
	printf("dirname = %s{%d}\n",ht->dirname,ht->dirname_size);
	puts("finish matching");
	if ( ht->sc_repository->type_id == REPOSITORY_TYPE_APT )
	{
		file_cache = 1;
	}

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
			printf("file_cache_path (%p) is %s\n", file_cache_path, file_cache_path);
			printf("file_cache_dir (%p) is %s\n", file_cache_dir, file_cache_dir);
			
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
		printf ("put into parser: %s\n", data);
		evhttp_parse_query_str(data, http_args);
	}
//	if ( querycodetype == EVHTTP_REQ_GET || querycodetype == EVHTTP_REQ_HEAD || querycodetype == EVHTTP_REQ_DELETE || querycodetype == EVHTTP_REQ_POST)
//	{
		if ( no_http_args == 0 )
		{
			for (i=0, http_arg = http_args->tqh_first; http_arg; http_arg = http_arg->next.tqe_next, i++)
			{
				printf("  %s: %s\n", http_arg->key, http_arg->value);
			}
			ht->args_len = i;
			ht->args = malloc(sizeof(http_kv)*ht->args_len);
			for (i=0, http_arg = http_args->tqh_first; http_arg; http_arg = http_arg->next.tqe_next, i++)
			{
				ht->args[i].key = http_arg->key;
				ht->args[i].value = http_arg->value;
				printf("COPYING %s and %s\n",ht->args[i].key,ht->args[i].value);
				printf("  %s: %s\n", http_arg->key, http_arg->value);
			}
		}
		else
		{
			ht->args_len = 0;
			ht->args = NULL;
		}
//	}
	ht->downloaduri = malloc(UCHAR_MAX);
	snprintf(ht->downloaduri,UCHAR_MAX-1,"http://%s/%s/%s",ht->host,ht->sc_repository->uri,ht->filepath);

	if ( ht->sc_repository->type_id == REPOSITORY_TYPE_YUM )
		rpmburner(ht);
	else if ( ht->sc_repository->type_id == REPOSITORY_TYPE_GENERIC )
		genericrepo(ht);
	else if ( ht->sc_repository->type_id == REPOSITORY_TYPE_APT )
		debburner(ht);
	else if ( ht->sc_repository->type_id == REPOSITORY_TYPE_DOCKER )
		genericrepo(ht);
	

	if ( querycodetype == EVHTTP_REQ_PUT || querycodetype == EVHTTP_REQ_POST )
		free(data);

	if ( file_cache == 1 )
	{
		free(file_cache_path);
	}

	puts("");
	evbuffer_add_printf(returnbuffer, "{\n \"repo\": \"%s\",\n \"path\": \"%s\",\n \"created\": \"%s\",\n \"createdBy\": \"%s\",\n \"downloadUri\": \"%s\",\n \"size\": %zu,\n \"checksums\":\n {\n  \"sha1\": \"%s\",\n  \"md5\":\"%s\"\n }\n}\n",ht->sc_repository->name, ht->filepath, "no", "no", ht->downloaduri, ht->data_size, "no","no");
	evhttp_send_reply(req, HTTP_OK, "Client", returnbuffer);
	evbuffer_free(returnbuffer);
	free(ht->downloaduri);
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
