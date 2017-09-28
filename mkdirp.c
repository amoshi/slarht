#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "mkdirp.h"

size_t get_rec_dir (char *str, size_t len, uint64_t num, int *fin)
{
	printf("go to %"PRIu64"\n",num);
	char *cursor = str, *cursor_tmp;
	uint64_t i;
	for ( i=0; i<num; i++ )
	{
		cursor_tmp = cursor;
		cursor = strstr(cursor, "/");
		if (cursor == cursor_tmp)
		{
			i--;
			cursor+=1;
			continue;
		}
		if (cursor == NULL)
		{
			*fin=1;
			return len;
		}
		cursor+=1;
		//printf("cursor(%p): %zu: %s\n",cursor,cursor-str,cursor);
	}
	//printf("return %zu\n", cursor-str);
	return cursor-str;
}

char * get_dir (char *str, uint64_t num, int *fin)
{
	size_t len = strlen(str);
	char *dir = malloc(len);
	size_t rc = get_rec_dir(str, len, num, fin);
	strncpy(dir,str,rc);
	dir[rc]='\0';
	return dir;
}

int mkdirp(char *dir)
{
	uint64_t i;
	int fin=0;
	char *r_dir;
	for ( i=0; fin!=1; i++ )
	{
		r_dir = get_dir (dir, i, &fin);
		mkdir(r_dir,S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
		free(r_dir);
	}
	return 0;
}

char *basename(char *str, int mod, size_t str_size)
{
	char *s;
	char *tmp;
	s = malloc(str_size);
	uint64_t b;
	for (tmp = str; ; tmp++ )
	{
		tmp = strstr(tmp,"/");
		if ( tmp == NULL )	break;
		b=tmp-str;
	}
	if ( mod == DIRBASENAME_D )
	{
		strncat(s,str,b);
	}
	else
	{
		strncat(s,str+b+1,str_size-b);
	}
	return s;
}
