//#include "evhttp.h"
#include "artifacts.h"
#include <limits.h>
int genericrepo(http_traf *ht)
{
	printf("==================================================\n");
	printf("genericrepo start\n");
	printf("HTTP full uri: %s\n", ht->full_uri);
	printf("HTTP query: %s\n", ht->query);
	printf("HTTP method: %s\n", ht->method);
	printf("HTTP method_id: %d\n", ht->method_id);
	//char base_yum_rpm_path[]="/var/www/repo/yum";
	char *repo_directory = ht->sc_repository->filesystem;
	char *command = malloc(UCHAR_MAX);
	snprintf(command,UCHAR_MAX,"createrepo --update %s",repo_directory);

	wrdata write_data;
	exdata exec_data;
	write_data.data = ht->data;
	write_data.data_size = ht->data_size;
	write_data.sc_repository = ht->sc_repository;
	write_data.dirname = ht->dirname;
	write_data.filename = ht->filename;
	exec_data.command = command;

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
	if ( ht->method_id == REQ_PUT )
	{
		int rc;
		if ( ( rc = artifact_write(&write_data) ) != 0 )	return rc;
	}
	printf("genericrepo stop\n");
	printf("==================================================\n");
}
