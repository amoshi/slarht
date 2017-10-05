#include <string.h>
#include <limits.h>
#include "artifacts.h"
#define EXECUTE_STDOUT_READLENGTH 30000
int artifact_write(wrdata *write_data)
{
	printf("HTTP PUT data(size %zu, %zu):\n%s\n", strlen(write_data->data), sizeof(write_data->data), write_data->data);
	FILE *artifact_dest;
	char artifact_dest_dir[FILENAME_MAX];
	char artifact_dest_path[FILENAME_MAX];
	if ( write_data->sc_repository->store_method == SLARHT_CONFIG_STORE_FILE )
	{
		snprintf(artifact_dest_dir,FILENAME_MAX-1,"%s/%s",write_data->sc_repository->filesystem,write_data->dirname);
		snprintf(artifact_dest_path,FILENAME_MAX-1,"%s/%s",artifact_dest_dir,write_data->filename);
	}
	mkdirp(artifact_dest_dir);
	if ( ( artifact_dest = fopen (artifact_dest_path, "w") ) == NULL )
	{
		fprintf(stderr, "Cannot open file: %s", artifact_dest_path );
		return 500;
	}
	fwrite(write_data->data,1,write_data->data_size,artifact_dest);
	//fprintf(artifact_dest,"%s",write_data->data);
	fclose(artifact_dest);
	return 0;
}

post_write_command(exdata *exec_data)
{
	FILE *repo_exec;
	int status;

	char *field = malloc(EXECUTE_STDOUT_READLENGTH);
	//snprintf(create_repo_cmd, 254, "repo --update %s", base_yum_rpm_path );
	printf("execute: %s\n", exec_data->command);
	repo_exec = popen(exec_data->command, "r");
	if (repo_exec == NULL)
		fprintf(stderr,"Failed to repo update: %s\n", exec_data->command);
	while (fgets(field, EXECUTE_STDOUT_READLENGTH-1, repo_exec) != NULL)
		printf("%s", field);
	status = pclose(repo_exec);
	free(field);
}

do_shell_script(slarht_conf_shell *sc_shell, int64_t sc_shell_size)
{
	FILE *script_exec;
	int status;
	char *field = malloc(EXECUTE_STDOUT_READLENGTH);
	if ( sc_shell_size == -1 )
	{
		printf("execute: %s\n", sc_shell->sc_script->scriptpath);
		script_exec = popen(sc_shell->sc_script->scriptpath, "r");
		if (script_exec == NULL)
			fprintf(stderr,"Failed execute script: %s\n", sc_shell->sc_script->scriptpath);
		while (fgets(field, EXECUTE_STDOUT_READLENGTH-1, script_exec) != NULL)
			printf("%s", field);
		status = pclose(script_exec);
	}
	free(field);
}
