#include "evhttp.h"
#include "artifacts.h"
#include <limits.h>
int repoburner(repo_conf *rconf)
{
	printf("==================================================\n");
	printf("repoburner start\n");
	printf("HTTP full uri: %s\n", rconf->ht->full_uri);
	printf("HTTP query: %s\n", rconf->ht->query);
	printf("HTTP method: %s\n", rconf->ht->method);
	printf("HTTP method_id: %d\n", rconf->ht->method_id);
	char *repo_directory = rconf->ht->sc_repository->filesystem;

	wrdata write_data;
	exdata exec_data;
	write_data.data = rconf->ht->data;
	write_data.data_size = rconf->ht->data_size;
	write_data.sc_repository = rconf->ht->sc_repository;
	write_data.dirname = rconf->ht->dirname;
	write_data.filename = rconf->ht->filename;
	exec_data.command = rconf->command;

	uint64_t i;
	puts("Headers:");
	for ( i=0; i < rconf->ht->headers_len; i++ )
	{
		printf("\t%s: %s\n", rconf->ht->headers[i].key, rconf->ht->headers[i].value);
	}
	puts("Args:");
	printf("len=%llu\n",rconf->ht->args_len);
	for ( i=0; i < rconf->ht->args_len; i++ )
	{
		printf("\t%s: %s\n", rconf->ht->args[i].key, rconf->ht->args[i].value);
	}
	puts("gogogo");
	do_shell_script(rconf->ht->sc_repository->between_script, rconf->ht->sc_repository->between_script_size, rconf->ht);
	if ( rconf->ht->method_id == REQ_PUT )
	{
		int rc;
		if ( rconf->command_size > 0 )
		{
			if ( ( rc = artifact_write(&write_data) ) != 0 )	return rc;
		}
		do_shell_script(rconf->ht->sc_repository->before_script, rconf->ht->sc_repository->before_script_size, rconf->ht);
		post_write_command(&exec_data);
	}
	do_shell_script(rconf->ht->sc_repository->after_script, rconf->ht->sc_repository->after_script_size, rconf->ht);
	printf("repoburner stop\n");
	printf("==================================================\n");
}
