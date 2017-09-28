//#include "evhttp.h"
#include "artifacts.h"
#include <limits.h>
int debburner(http_traf *ht)
{
	printf("==================================================\n");
	printf("debburner start\n");
	printf("HTTP full uri: %s\n", ht->full_uri);
	printf("HTTP query: %s\n", ht->query);
	printf("HTTP method: %s\n", ht->method);
	printf("HTTP method_id: %d\n", ht->method_id);
	char *repo_directory = ht->sc_repository->filesystem;
	char *command = malloc(UCHAR_MAX);

	wrdata write_data;
	exdata exec_data;
	exec_data.command = malloc(UCHAR_MAX);
	write_data.data = ht->data;
	write_data.data_size = ht->data_size;
	write_data.sc_repository = ht->sc_repository;
	write_data.dirname = ht->dirname;
	write_data.filename = ht->filename;

	uint64_t i;
	puts("Headers:");
	for ( i=0; i < ht->headers_len; i++ )
	{
		printf("\t%s: %s\n", ht->headers[i].key, ht->headers[i].value);
	}
	puts("Args:");
	printf("len=%llu\n",ht->args_len);
	for ( i=0; i < ht->args_len; i++ )
	{
		printf("\t%s: %s\n", ht->args[i].key, ht->args[i].value);
	}
	puts("gogogo");
	printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->name=%s\n",i,ht->sc_repository->name);
	printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->type=%s\n",i,ht->sc_repository->type);
	printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->type_id=%d\n",i,ht->sc_repository->type_id);
	printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->max_version_artifacts=%"PRIu64"\n",i,ht->sc_repository->max_version_artifacts);
	printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->max_size=%"PRIu64"\n",i,ht->sc_repository->max_size);
	printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->store_method=%d\n",i,ht->sc_repository->store_method);
	printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->old_version_repo=%s\n",i,ht->sc_repository->old_version_repo);
	printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->filesystem=%s\n",i,ht->sc_repository->filesystem);
	printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->storage=%s\n",i,ht->sc_repository->storage);
	printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->uri=%s\n",i,ht->sc_repository->uri);
	printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->uri_size=%"PRIu64"\n",i,ht->sc_repository->uri_size);
	printf("file_cache_path (%p) '%s'\n", ht->file_cache_path, ht->file_cache_path);
	if ( ht->method_id == REQ_PUT )
	{
		int rc;
		snprintf(exec_data.command,UCHAR_MAX,"reprepro -b '%s' -C main includedeb general '%s'",ht->sc_repository->filesystem,ht->file_cache_path);
		post_write_command(&exec_data);
	}
	free(exec_data.command);
	printf("debburner stop\n");
	printf("==================================================\n");
}
