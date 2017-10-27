#include <string.h>
#include <limits.h>
#include "artifacts.h"
#include "mkdirp.h"
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
	printf("writing file: %s\n", artifact_dest_path);
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

int artifact_pushtodir( pushdir_t *pshdir )
{
	printf("push cached file %s to %s\n", pshdir->from, pshdir->to);
	FILE *artifact_dest;
	char dest[FILENAME_MAX];
	snprintf(dest,FILENAME_MAX,"%s/%s",pshdir->sc_repository->filesystem,pshdir->to);
	char *artifact_dest_dir = basename(dest, DIRBASENAME_D,strlen(dest));
	char *artifact_dest_base = basename(dest, DIRBASENAME_B,strlen(dest));
	mkdirp(artifact_dest_dir);
	rename ( pshdir->from, dest );

	free (artifact_dest_dir);
	free (artifact_dest_base);
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

do_shell_script(slarht_conf_shell *sc_shell, int64_t sc_shell_size, http_traf *ht)
{
	if (sc_shell_size < 1 && sc_shell_size !=-1)
	{
		puts("nothing to do");
		return;
	}
	FILE *script_exec;
	size_t execute_size = SHRT_MAX;
	char *execute = malloc(execute_size);
	printf("export FILENAME=\"%s\"; export FILEPATH=\"%s\"; export DIRNAME=\"%s\"; export QUERY=\"%s\"; export FULL_URI=\"%s\"; export HTTP_METHOD=\"%s\"; export REPOSITORY_DIR=\"%s\"; export REPOSITORY_NAME=\"%s\"; export HTTP_HOST=\"%s\"; export DOWNLOADURI=\"%s\"; export TEMPLATE=\"%s\"; %s\n",
					ht->filename,
					ht->filepath,
					ht->dirname,
					ht->query,
					ht->full_uri,
					ht->method,
					ht->sc_repository->filesystem,
					ht->sc_repository->name,
					ht->host,
					ht->downloaduri,
					ht->_template,
					sc_shell->sc_script->scriptpath);
	snprintf(execute,execute_size-1,"export FILENAME=\"%s\"; export FILEPATH=\"%s\"; export DIRNAME=\"%s\"; export QUERY=\"%s\"; export FULL_URI=\"%s\"; export HTTP_METHOD=\"%s\"; export REPOSITORY_DIR=\"%s\"; export REPOSITORY_NAME=\"%s\"; export HTTP_HOST=\"%s\"; export DOWNLOADURI=\"%s\"; export TEMPLATE=\"%s\"; %s",
					ht->filename,
					ht->filepath,
					ht->dirname,
					ht->query,
					ht->full_uri,
					ht->method,
					ht->sc_repository->filesystem,
					ht->sc_repository->name,
					ht->host,
					ht->downloaduri,
					ht->_template,
					sc_shell->sc_script->scriptpath);
	int status;
	char *field = malloc(EXECUTE_STDOUT_READLENGTH);
	if ( sc_shell_size == -1 )
	{
		printf("execute: %s\n", execute);
		script_exec = popen(execute, "r");
		if (script_exec == NULL)
			fprintf(stderr,"Failed execute script: %s\n", execute);
		while (fgets(field, EXECUTE_STDOUT_READLENGTH-1, script_exec) != NULL)
			printf("%s", field);
		status = pclose(script_exec);
	}
	free(field);
	free(execute);
}
