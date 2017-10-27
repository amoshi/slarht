#include <sys/types.h>
#include <errno.h>
#include <dirent.h>

void create_pypi_index(char *dirname)
{
	puts("CREATE INDEX");
	DIR *dir;
	char indexhtml[PATH_MAX];
	char indexdir[PATH_MAX];
	snprintf(indexdir, PATH_MAX, "%s/.pypi", dirname);
	snprintf(indexhtml, PATH_MAX, "%s/simple.html", indexdir);
	printf("dirname=%s\n", dirname);
	printf("indexdir=%s\n", indexdir);
	printf("indexhtml=%s\n", indexhtml);
	mkdirp(dirname);
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
	free(ht->filepath);
	free(ht->dirname);
	free(ht->filename);
	ht->file_cache_path = pstdat->fkv->value;
	printf("filacache=%s\n",ht->file_cache_path);
	ht->filepath = malloc ( FILENAME_MAX );
	snprintf(ht->filepath, FILENAME_MAX, "/%s/%s/%s", pstdat->gv.name, pstdat->gv.version, pstdat->fkv->key);
	printf("string is /%s/%s/%s\n", pstdat->gv.name, pstdat->gv.version, pstdat->fkv->key);
	printf("name=%s\nversion=%s\nfilename=%s\n", pstdat->gv.name, pstdat->gv.version, pstdat->fkv->key);
	printf("fil e path is %s\n", ht->filepath);
	ht->filepath_size = strlen(ht->filepath);
	puts("1");
	ht->filename = basename(ht->filepath,DIRBASENAME_B,ht->filepath_size);
	puts("2");
	ht->filename_size = strlen(ht->filename);
	puts("3");
	if ( ht->filename_size == 0 )
	{
	puts("4");
		snprintf(ht->filename,2,"%s","t");
	puts("5");
		ht->filename_size=2;
	puts("6");
	}
	puts("7");
	ht->dirname = basename(ht->filepath,DIRBASENAME_D,ht->filepath_size);
	puts("8");
	ht->dirname_size = strlen(ht->dirname);
	puts("9");

	repo_conf *rconf = malloc(sizeof(repo_conf));
	puts("10");
	size_t command_size = 0;
	puts("11");

	rconf->deploy_method_post = DEPLOY_METHOD_POST_FUNC;
	printf ( "pypi method_post=%d\n",rconf->deploy_method_post );
	rconf->deploy_method=DEPLOY_METHOD_PUSHTODIR;
	rconf->func=&create_pypi_index;
	rconf->arg = ht->sc_repository->filesystem;
	printf("filesystem=%s\n",rconf->arg);
	puts("12");
	rconf->command=NULL;
	puts("13");
	rconf->ht=ht;
	puts("14");
	printf("exit pypi\n");
	return rconf;
}
