#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "mkdirp.h"

size_t get_rec_dir (char *str, size_t len, uint64_t num, int *fin)
{
	//printf("go to %"PRIu64"\n",num);
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
	if (str_size < 2)	str_size+=2;
	s = malloc(str_size);
	if ( str_size < 3 )
	{
		return s;
	}
	uint64_t b;
	for (tmp = str; ; tmp++ )
	{
		tmp = strstr(tmp,"/");
		if ( tmp == NULL )	break;
		b=tmp-str;
	}
	if ( mod == DIRBASENAME_D )
	{
		strncpy(s,str,b);
		s[b]='\0';
	}
	else
	{
		strncpy(s,str+b+1,str_size-b);
	}
	return s;
}

char *gen_tmp_filename(char *dir, char *dirname, char *fname)
{
	char *file_cache_path = malloc(FILENAME_MAX);
	char file_cache_dir[FILENAME_MAX];
	struct timespec now_time;
	clock_gettime(CLOCK_REALTIME, &now_time);
	snprintf(file_cache_dir,FILENAME_MAX,"%s/%ld.%09ld/%s",dir,now_time.tv_sec,now_time.tv_nsec,dirname);
	snprintf(file_cache_path,FILENAME_MAX,"%s/%s",file_cache_dir,fname);
	mkdirp(file_cache_dir);
	fprintf(stdout, "file_cache_path (%p) is %s\n", file_cache_path, file_cache_path);
	fprintf(stdout, "file_cache_dir (%p) is %s\n", file_cache_dir, file_cache_dir);
	return file_cache_path;
}

int filetype(const char *path)
{
	struct stat path_stat;
	stat(path, &path_stat);
	if ( S_ISREG(path_stat.st_mode) == 1 )
		return ISTAT_FILE;
	if ( S_ISDIR(path_stat.st_mode) == 1 )
		return ISTAT_DIRECTORY;
	if ( S_ISCHR(path_stat.st_mode) == 1 )
		return ISTAT_CHR;
	if ( S_ISBLK(path_stat.st_mode) == 1 )
		return ISTAT_BLOCKDEVICE;
	if ( S_ISFIFO(path_stat.st_mode) == 1 )
		return ISTAT_FIFO;
	if ( S_ISSOCK(path_stat.st_mode) == 1 )
		return ISTAT_SOCKET;
	else
		return ISTAT_NOTFOUND;
}
