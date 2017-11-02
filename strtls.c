#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
char* copy_init(char *str)
{
	char *ret;
	size_t len = strlen(str);
	ret = calloc(1,len);
	ret[0]='\0';
	strncpy(ret,str,len);
	return ret;
}

char* copy_init_n(char *str, size_t len)
{
	char *ret;
	ret = calloc(1,len);
	ret[0]='\0';
	strncpy(ret,str,len);
	return ret;
}

int64_t charfind(char *str, char c)
{
	uint64_t i;
	size_t n = strlen ( str );
	for ( i=0; i<n && str[i]!=c; i++ );
	if ( str[i]!=c )	return -1;
	else			return i;
}

size_t charfind_n(char *str, char c, size_t n)
{
	uint64_t i;
	for ( i=0; i<n && str[i]!=c; i++ );
	return i;
}

char *strstr_fn(char *s, char *m, size_t n)
{
	puts("________strstr_fn");
	printf("s=%s\n", s);
	printf("m=%s\n", m);
	uint64_t i, j, k, tmp;
	int rc;
	size_t m_size = strlen(m);
	if ( m_size > n )
	{
		//printf("0return NULL\n");
		return NULL;
	}
	for ( i=0; i<n-m_size+1; i++ )
	{
		k = 0;
		j=i;
		tmp = n-i;
		//printf("tmp=%"PRIu64"\n",tmp);
		for ( ; j<tmp; j++, k++ )
		{
			//printf("i=%"PRIu64", j=%"PRIu64", k=%"PRIu64", n=%zu, size=%zu\n", i,j,k, n,m_size);
			if ( s[j]!=m[k] )
			{
				//printf("%c != %c, EXIT\n",s[j], m[k]);
				break;
			}
			//else	printf("%c == %c\n",s[j], m[k]);
			if ( k>=m_size )
			{
				//printf("1return %p\n", s+i);
				return s+i;
			}
		}
		//printf("K=%"PRIu64",size=%zu\n", k, m_size);
		if ( k == m_size )
		{
			//printf("2return %p\n", s+i);
			return s+i;
		}
		//if ( ( rc = memcmp(s, m, m_size) ) == 0 )	return s+i;
		//printf("s m rc=%d\n",rc);
	}
	//printf("3return NULL\n");
	return NULL;
}
