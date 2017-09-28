#include <inttypes.h>
#include <errno.h>
#include <stdio.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "config_yaml.h"

int dir_create_check_p(char *dirname)
{
	struct stat sb;

	if (stat(dirname, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		printf("YES\n");
	}
	else
	{
		printf("NO\n");
		DIR* dir = opendir(dirname);
		if (dir)
		{
			/* Directory exists. */
			closedir(dir);
		}
		else if (ENOENT == errno)
		{
			mkdirp(dirname);
		}
		else
		{
			fprintf(stderr,"%s directory cannot open\n",dirname);
		}
	}
}

uint64_t repcheck(slarht_conf_general *sc_general)
{
	uint64_t i;
	size_t repsize = sc_general->sc_repository_size;
	for ( i=0; i<repsize; i++ )
	{
		mkdirp(sc_general->scp_repository[i].sc_repository->filesystem);
		if ( sc_general->scp_repository[i].sc_repository->type_id == REPOSITORY_TYPE_APT )
		{
			char metadatadir[UCHAR_MAX];
			char metadata[UCHAR_MAX];
			snprintf(metadatadir,UCHAR_MAX-1,"%s/conf",sc_general->scp_repository[i].sc_repository->filesystem);
			snprintf(metadata,UCHAR_MAX-1,"%s/distributions",metadatadir);
			mkdirp(metadatadir);
			FILE *fd;
			if ( ( fd = fopen(metadata,"r") ) == NULL )
			{
				if ( ( fd = fopen(metadata,"w") ) == NULL )
				{
					fprintf(stderr, "Cannot create file %s\n", metadata);
					return -1;
				}
				else
				{
					fprintf(fd,"Codename: general\nSuite: stable\nVersion: stable\nOrigin: Slarht deb repository\nLabel: Slarht deb repository\nDescription: Slarht deb repository\nArchitectures: source i386 amd64\nComponents: main\nSignWith: default\nDebIndices: Packages Release . .gz .bz2\nDscIndices: Sources Release . .gz .bz2\nContents: . .gz .bz2\n");
					fclose(fd);
				}
			}
		}
	}
}
