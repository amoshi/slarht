#include "evhttp.h"
#define POST_MULTI_DATA_FROM_FILE 0
#define POST_MULTI_DATA_FROM_CHAR 1
#define POST_STRLEN_SIZE 1000
#define POST_MULTI_CONTENT_DISPOSITION "Content-Disposition:"
#define POST_MULTIPART_BOUNDARY "boundary"
#define POST_MULTIPART_BOUNDARY_SIZE 8

typedef struct genvalues
{
	char *name;
	char *version;
} genvalues;

typedef struct postdata
{
	http_kv *kv;
	size_t kv_size;
	http_kv *fkv;
	size_t fkv_size;
	genvalues gv;
} postdata;

postdata* post_multipart_parse(char *data, size_t len, int mode, char *boundary, size_t boundary_size, char *tmp_dir);
