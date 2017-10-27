#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "post.h"
#include "mstr.h"
#include "evhttp.h"


mstr get_dispdata_name(char *field, char *sep)
{
	mstr ms;
	char *s = field+strlen(POST_MULTI_CONTENT_DISPOSITION);
	char *p;
	uint64_t field_size = strlen(s);
	//printf("s=%s(%"PRIu64")\n",s,field_size);
	uint64_t i;
	for ( i=0; i<field_size && s[i]!=';'; i++ );
	p = strstr(s,sep);
	if ( p == NULL )
	{
		ms.s = malloc(1);
		ms.s[0]='\0';
		ms.l = 0;
	}
	else
	{
		p+=strlen(sep);
		s = p;
		//printf("searching %s in %s\n","\"", s);
		p = strstr(s, "\"");
		if ( p == NULL )
		{
			ms.s = malloc(1);
			ms.s[0]='\0';
			ms.l = 0;
		}
		//printf("dif: %p - %p\n",p,s);
		i = p-s;
		ms.s = malloc (i);
		printf("copying %s with length %"PRIu64"\n",s,i);
		strncpy(ms.s,s,i);
		ms.s[i]='\0';
		ms.l = i;
	}

	return ms;
}

mstr get_dispdata_type(char *field)
{
	mstr ms;
	char *s = field+strlen(POST_MULTI_CONTENT_DISPOSITION);
	uint64_t field_size = strlen(s);
	uint64_t i;
	char *f = s;
	for ( i=0; i<field_size && ( s[i]==';' || s[i]==' ' || s[i]=='\t' ); i++ );
	f = s+i;
	for ( i=0; i<field_size && f[i]!=';' && f[i]!=' ' && f[i]!='\t'; i++ );
	ms.s = malloc (i);
	strncpy(ms.s,f,i);
	ms.s[i]='\0';
	ms.l = i;

	return ms;
}

postdata* post_multipart_parse(char *data, size_t len, int mode, char *boundary, size_t boundary_size, char *tmp_dir)
{
	puts("---------------------------------------------------------------------------------------------");
	printf( " POST query handler: file %s, len = %zu, mode %d, boundary '%s' (%zu), tmp_dir %s\n", data, len, mode, boundary, boundary_size, tmp_dir );
	postdata *pstdat = malloc(sizeof(postdata));
	http_kv *kv = malloc(HTTP_ARGS_MAXCOUNT*sizeof(http_kv));
	http_kv *fkv = malloc(HTTP_ARGS_MAXCOUNT*sizeof(http_kv));
	int64_t cnt=0,fcnt=0;
	if ( mode == POST_MULTI_DATA_FROM_FILE )
	{
		char *field = malloc ( POST_STRLEN_SIZE );
		FILE *fd = fopen(data,"r");
		FILE *f_out;
		char *f_outfile_name = malloc(FILENAME_MAX);
		if ( fd == NULL )
		{
			return NULL;
		}

		int dispsize = strlen(POST_MULTI_CONTENT_DISPOSITION);
		mstr dispdata_type;
		mstr dispdata_name;
		mstr dispdata_filename;
		for ( ; fgets(field, POST_STRLEN_SIZE-1, fd); )
		{
			puts("===================================================================================================================");
			size_t field_size=strlen(field);

			if ( startswith_matches_n(field, POST_MULTI_CONTENT_DISPOSITION, dispsize) == dispsize )
			{
				dispdata_type = get_dispdata_type(field);
				dispdata_name = get_dispdata_name(field+dispdata_type.l, "; name=\"");
				dispdata_filename = get_dispdata_name(field+dispdata_type.l+dispdata_name.l, "filename=\"");
				//printf("type=%s(%"PRIu64")\n",dispdata_type.s,dispdata_type.l);
				//printf("name=%s(%"PRIu64")\n",dispdata_name.s,dispdata_name.l);
				//printf("filename=%s(%"PRIu64")\n",dispdata_filename.s,dispdata_filename.l);
				size_t stt;
				if ( dispdata_filename.l > 0 )
				{
					fkv[fcnt].key=dispdata_filename.s;
					fkv[fcnt].key_size=dispdata_filename.l;
					snprintf(f_outfile_name, FILENAME_MAX, "%s/%s", tmp_dir, dispdata_filename.s);
					printf("open %s for write\n",f_outfile_name);
					f_out = fopen(f_outfile_name, "w");
					fkv[fcnt].value=f_outfile_name;
					fkv[fcnt].key_size=strlen(f_outfile_name);
					fgets(field, POST_STRLEN_SIZE-1, fd);
					char *st;
					size_t rc;
					uint64_t rb;
					//while ( ( stt = startswith_matches_n(field+2, boundary, boundary_size) ) != boundary_size )
					while ( !feof(fd) )//&& ( st = strstr(field,boundary) ) == NULL  ) //|| ( stt = startswith_matches_n(field+2,boundary,boundary_size) ) == boundary_size )
					{
						rc = fread(field, 1, POST_STRLEN_SIZE-1, fd);
						st = strstr_fn(field,boundary,rc);
						if ( st == NULL )
							rb = POST_STRLEN_SIZE-1;
						else
							rb = st - field;
						//rb = st ? POST_STRLEN_SIZE-1 : (st-field);
						fwrite(field, 1, rb, f_out);
						//if ( fgets(field, POST_STRLEN_SIZE-1, fd) == NULL )
						//	break;
						if ( st != NULL )
						{
							printf("exit strstr result is %p\n",st);
							break;
						}
					}
					//printf("diff: %zu\n",stt);
					fwrite(field, 1, rb, f_out);
					fclose(f_out);
					printf("file %s writed\n",dispdata_filename.s);
					fseek(fd, rb-rc, SEEK_CUR);
					fcnt++;
				}
				else
				{
					int64_t carsym;
					kv[cnt].value_size=0;
					kv[cnt].key=dispdata_name.s;
					kv[cnt].key_size=dispdata_name.l;
					fgets(field, POST_STRLEN_SIZE-1, fd);
					kv[cnt].value = malloc(POST_STRLEN_SIZE);
					kv[cnt].value[0]='\0';
					fgets(field, POST_STRLEN_SIZE-1, fd);
					while ( ( stt = startswith_matches_n(field+2, boundary, boundary_size) ) != boundary_size )
					{
						if ( stt == boundary_size ) break;
						if ( ( carsym = charfind (field, '\r') ) != -1 )
							field[carsym]='\0';
						if ( ( carsym = charfind (field, '\n') ) != -1 )
							field[carsym]='\0';
						field_size = strlen(field);
						//printf("cnt=%s\n",kv[cnt].value);
						strncat(kv[cnt].value,field,field_size);
						kv[cnt].value_size+=field_size;
						
						if ( fgets(field, POST_STRLEN_SIZE-1, fd) == NULL )
							break;
					}

					if ( !strcmp(kv[cnt].key,"name") )
						pstdat->gv.name=kv[cnt].value;
					if ( !strcmp(kv[cnt].key,"version") )
						pstdat->gv.version=kv[cnt].value;

					cnt++;
				}
			}
			else if (startswith_matches_n(field,"\r\n",2) == 2 ) {}
			else if (startswith_matches_n(field,"\n",1) == 1 ) {}
		}
	}
	else if ( mode == POST_MULTI_DATA_FROM_CHAR )
	{
	}
	pstdat->kv = kv;
	pstdat->fkv = fkv;
	pstdat->kv_size = cnt;
	pstdat->fkv_size = fcnt;
	puts("exit post parser");
	printf("name=%s\nversion=%s\n", pstdat->gv.name, pstdat->gv.version);
	return pstdat;
}


//void main(int argc, char **argv)
//{
//	printf("file: %s\n",argv[1]);
//	int i;
////	postdata *pstdat = post_multipart_parse(argv[1], 0, POST_MULTI_DATA_FROM_FILE, "--------------GHSKFJDLGDS7543FJKLFHRE75642756743254", strlen("--------------GHSKFJDLGDS7543FJKLFHRE75642756743254"), "/var/slarht/");
//	postdata *pstdat = post_multipart_parse(argv[1], 0, POST_MULTI_DATA_FROM_FILE, "--------------GHSKFJDLGDS7543FJKLFHRE75642756743254", strlen("--------------GHSKFJDLGDS7543FJKLFHRE75642756743254"), "/var/slarht/");
//	printf("kv_size=%"PRId64"\n", pstdat->kv_size);
//	for ( i=0; i<pstdat->kv_size;
//		printf("kv[%d].key=%s(%zu)\n", i, pstdat->kv[i].key, pstdat->kv[i].key_size),
//		printf("kv[%d].value=%s(%zu)\n", i, pstdat->kv[i].value, pstdat->kv[i].value_size),
//		i++
//	);
//	printf("fkv_size=%"PRId64"\n", pstdat->fkv_size);
//	for ( i=0; i<pstdat->fkv_size;
//		printf("fkv[%d].key=%s(%zu)\n", i, pstdat->fkv[i].key, pstdat->fkv[i].key_size),
//		printf("fkv[%d].value=%s(%zu)\n", i, pstdat->fkv[i].value, pstdat->fkv[i].value_size),
//		i++
//	);
//}

