#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

void create_pypi_index(char *dirname)
{
	puts("CREATE INDEX");
	DIR *dir;
	char indexhtml[FILENAME_MAX];
	char indexdir[FILENAME_MAX];
	snprintf(indexdir, FILENAME_MAX, "%s/.pypi", dirname);
	snprintf(indexhtml, FILENAME_MAX, "%s/simple.html", indexdir);
	printf("dirname=%s\n", dirname);
	printf("indexdir=%s\n", indexdir);
	printf("indexhtml=%s\n", indexhtml);
	//mkdirp(indexdir);
	mkdir(indexdir,S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	FILE *index;
	if (  ( index = fopen(indexhtml, "w") ) == NULL )
	{
		printf("%s: ",indexhtml);
		perror("fopen");
		return;
	}
	
	struct dirent *entry;

	dir = opendir(dirname);
	if (!dir)
	{
		printf("%s: ",dirname);
		perror("opendir");
	}
	else
	{
		fprintf(index,"<html><head><title>Simple Index</title><meta name=\"api-version\" value=\"2\" /></head><body>\n");
		while ( (entry = readdir(dir)) != NULL)
		{
			if ( entry->d_name[0]!='.' )
				fprintf(index,"<a href=\"%s\" rel=\"internal\" >%s</a><br/>\n", entry->d_name, entry->d_name);
		}
		fprintf(index,"</body></html>\n");

		closedir(dir);
	}
	fclose(index);
	puts("END INDEX");
}

repo_conf* pypi_conf(http_traf *ht, postdata *pstdat)
{
	uint64_t i;
	free(ht->filepath);
	free(ht->dirname);
	free(ht->filename);
	ht->file_cache_path = pstdat->fkv->value;
	printf("filacache=%s\n",ht->file_cache_path);
	ht->filepath = malloc ( FILENAME_MAX );
	snprintf(ht->filepath, FILENAME_MAX, "/%s/%s/%s", pstdat->gv.name, pstdat->gv.version, pstdat->fkv->key);
	for(i = 0; ht->filepath[i]; i++){
		ht->filepath[i] = tolower(ht->filepath[i]);
	}
	printf("string is /%s/%s/%s\n", pstdat->gv.name, pstdat->gv.version, pstdat->fkv->key);
	printf("name=%s\nversion=%s\nfilename=%s\n", pstdat->gv.name, pstdat->gv.version, pstdat->fkv->key);
	printf("fil e path is %s\n", ht->filepath);
	ht->filepath_size = strlen(ht->filepath);
	ht->filename = basename(ht->filepath,DIRBASENAME_B,ht->filepath_size);
	ht->filename_size = strlen(ht->filename);
	if ( ht->filename_size == 0 )
	{
		snprintf(ht->filename,2,"%s","t");
		ht->filename_size=2;
	}
	ht->dirname = basename(ht->filepath,DIRBASENAME_D,ht->filepath_size);
	ht->dirname_size = strlen(ht->dirname);

	repo_conf *rconf = malloc(sizeof(repo_conf));
	size_t command_size = 0;

	rconf->deploy_method_post = DEPLOY_METHOD_POST_FUNC;
	rconf->deploy_method=DEPLOY_METHOD_PUSHTODIR;
	rconf->func=&create_pypi_index;
	rconf->arg = ht->sc_repository->filesystem;
	rconf->command=NULL;
	rconf->ht=ht;
	printf("exit pypi\n");
	return rconf;
}
