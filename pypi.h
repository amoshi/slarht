#include <sys/types.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "get.h"

void create_pypi_index(char *dirname, char *url);

void create_pypi_index(char *dirname, char *url)
{
	puts("CREATE INDEX");
	DIR *dir, *dir2;
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
	if ( ( index = fopen(indexhtml, "w") ) == NULL )
	{
		printf("%s: ",indexhtml);
		perror("fopen");
		return;
	}
	
	struct dirent *entry;

	printf("\n0trying to open %s\n",dirname);
	dir = opendir(dirname);
	if (!dir)
	{
		printf("%s: ",dirname);
		perror("opendir");
	}
	else
	{
		fprintf(index, "<!DOCTYPE html><html><head><title>Simple Index</title></head><body><h1>Simple Index</h1>\n");
		while ( (entry = readdir(dir)) != NULL)
		{
			if ( ( entry->d_name[0]=='.' && entry->d_name[1]=='.' ) || ( entry->d_name[0]=='.' && entry->d_name[1]=='\0' ) )
				continue;
			if ( !strcmp(entry->d_name, ".pypi" ) )
				continue;
			char dirname2[FILENAME_MAX];
			printf("dirname=%s\n", dirname);
			printf("entry=%s\n", entry->d_name);
			snprintf(dirname2, FILENAME_MAX, "%s/%s", dirname, entry->d_name );
			printf("\n1trying to open %s\n",dirname2);
			dir2 = opendir(dirname2);
			struct dirent *entry2;
			if (!dir2)
			{
				printf("%s: ",dirname2);
				perror("opendir");
			}
			else
			{
				while ( (entry2 = readdir(dir2)) != NULL)
				{
					if ( ( entry2->d_name[0]=='.' && entry2->d_name[1]=='.' ) || ( entry2->d_name[0]=='.' && entry2->d_name[1]=='\0' ) )
						continue;
					if ( !strcmp(entry2->d_name, ".pypi" ) )
						continue;

					char index2[FILENAME_MAX];
					snprintf(index2, FILENAME_MAX, "%s/%s", dirname2, INDEXFILE );
					FILE *indfd;
					DIR *dir3;
					printf("open file for writing: %s\n",index2);
					indfd = fopen(index2, "w");
					fprintf(indfd, "<!DOCTYPE html><html><head><title>Links for %s</title></head><body><h1>Links for %s</h1>\n", entry->d_name, entry->d_name);
					char dirname3[FILENAME_MAX];
			 	 	snprintf(dirname3, FILENAME_MAX, "%s/%s", dirname2, entry2->d_name );
					printf("\n2trying to open: %s\n ",dirname3);
			 	 	dir3 = opendir(dirname3);
			 	 	if (!dir3)
			 	 	{
			 	 		printf("%s: ",dirname3);
			 	 		perror("opendir");
			 	 	}
			 	 	else
			 	 	{
						struct dirent *entry3;
			 	 		while ( (entry3 = readdir(dir3)) != NULL)
			 	 		{
							if ( ( entry3->d_name[0]=='.' && entry3->d_name[1]=='.' ) || ( entry3->d_name[0]=='.' && entry3->d_name[1]=='\0' ) )
								continue;

							fprintf(indfd, "<a href=\"%s/%s/%s/%s\" rel=\"internal\">%s</a></br/>\n", url, entry->d_name, entry2->d_name, entry3->d_name, entry3->d_name);
						}
					}
					fprintf(indfd,"</body></html>\n");
					fclose(indfd);
				}
			}

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
	rconf->arg2 = ht->sc_repository->uri;
	rconf->command=NULL;
	rconf->ht=ht;
	printf("exit pypi\n");
	return rconf;
}

pypi_init(http_traf *ht)
{
	ht->dir_to_index = 1;
}
