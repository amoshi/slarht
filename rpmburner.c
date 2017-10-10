//#include "evhttp.h"
#include "artifacts.h"
#include <limits.h>
int rpmburner(http_traf *ht)
{
	printf("==================================================\n");
	printf("rpmburner start\n");
	printf("HTTP full uri: %s\n", ht->full_uri);
	printf("HTTP query: %s\n", ht->query);
	printf("HTTP method: %s\n", ht->method);
	printf("HTTP method_id: %d\n", ht->method_id);
        printf("FILEPATH=%s\n",ht->filepath);
        printf("FILENAME=%s\n",ht->filename);
        printf("DIRNAME=%s\n",ht->dirname);
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
	//if ( ht->method_id == REQ_PUT )
	//{
	//	//printf("HTTP PUT data(size %zu, %zu):\n%s\n", strlen(ht->data), sizeof(ht->data), ht->data);
	//	FILE *rpm_dest;
	//	char rpm_dest_path[255];
	//	snprintf(rpm_dest_path,255,"%s/%s",base_yum_rpm_path,ht->query);
	//	if ( ( rpm_dest = fopen (rpm_dest_path, "w") ) == NULL )
	//	{
	//		fprintf(stderr, "Cannot open file: %s", rpm_dest_path );
	//		return 500;
	//	}
	//	fwrite(ht->data,1,ht->data_size,rpm_dest);
	//	//fprintf(rpm_dest,"%s",ht->data);
	//	fclose(rpm_dest);



	//	FILE *createrepo_exec;
	//	int status;

	//	char create_repo_cmd[255], field[255];
	//	snprintf(create_repo_cmd, 254, "createrepo --update %s", base_yum_rpm_path );
	//	createrepo_exec = popen(create_repo_cmd, "r");
	//	if (createrepo_exec == NULL)
	//		fprintf(stderr,"Failed to createrepo update: %s\n", create_repo_cmd);
	//	while (fgets(field, 1000, createrepo_exec) != NULL)
    	//		printf("%s", field);
	//	status = pclose(createrepo_exec);

	//}
	do_shell_script(ht->sc_repository->between_script, ht->sc_repository->between_script_size, ht);
	if ( ht->method_id == REQ_PUT )
	{
		int rc;
		if ( ( rc = artifact_write(&write_data) ) != 0 )	return rc;
		do_shell_script(ht->sc_repository->before_script, ht->sc_repository->before_script_size, ht);
		post_write_command(&exec_data);
	}
	do_shell_script(ht->sc_repository->after_script, ht->sc_repository->after_script_size, ht);
	printf("rpmburner stop\n");
	printf("==================================================\n");
}
