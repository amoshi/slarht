#define DIRBASENAME_B 0
#define DIRBASENAME_D 1

#define ISTAT_FILE 0
#define ISTAT_DIRECTORY 1
#define ISTAT_CHR 2
#define ISTAT_BLOCKDEVICE 3
#define ISTAT_FIFO 4
#define ISTAT_SOCKET 5
#define ISTAT_SYMLINK 6
#define ISTAT_NOTFOUND 7

char *basename(char *str, int mod, size_t str_size);
char *gen_tmp_filename(char *dir, char *dirname, char *fname);
