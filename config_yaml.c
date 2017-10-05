#include <stdio.h>
#include <yaml.h>
#include <stdbool.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include "config_yaml.h"

char* copy_init(char *str)
{
	char *ret;
	size_t len = strlen(str)+1;
	ret = calloc(1,len);
	ret[0]='\0';
	strncpy(ret,str,len-1);
	return ret;
}

char* copy_init_n(char *str, size_t len)
{
	char *ret;
	ret = calloc(1,len);
	ret[0]='\0';
	strncpy(ret,str,len-1);
	return ret;
}

slarht_conf_user *sc_user_init(size_t num)
{
        slarht_conf_user *sc_user = calloc(sizeof(slarht_conf_user),1);
        uint64_t i;
        sc_user->id=num;
        sc_user->encryption_type=0;
        return sc_user;
} 

slarht_conf_storage *sc_storage_init()
{
        slarht_conf_storage *sc_storage = calloc(sizeof(slarht_conf_storage),1);
        sc_storage->id=0;
        sc_storage->type=0;
        return sc_storage;
}

slarht_conf_repository *sc_repository_init()
{
        slarht_conf_repository *sc_repository = calloc(sizeof(slarht_conf_repository),1);
        sc_repository->id=0;
        sc_repository->type=0;
        sc_repository->max_size=0;
        sc_repository->max_version_artifacts=0;
        return sc_repository;
}

//typedef struct slarht_conf_general
//{
//        int config_source;
//        char *config_path;
//        slarth_conf_ptr_user scp_user[MAX_USER_SIZE];
//        slarth_conf_ptr_repository scp_repository[MAX_REPOSITORY_SIZE];
//        slarth_conf_ptr_storage scp_storage[MAX_STORAGE_SIZE];
//        size_t sc_user_size;
//        size_t sc_repository_size;
//        size_t sc_storage_size;
//} slarht_conf_general;

void sc_general_print(slarht_conf_general *sc_general)
{
	printf("dump sc_general(%p)\n",sc_general);
	printf("config_source=%d\n",sc_general->config_source);
	uint64_t i, j;
	uint64_t sc_user_size = sc_general->sc_user_size;
	uint64_t sc_storage_size = sc_general->sc_storage_size;
	uint64_t sc_repository_size = sc_general->sc_repository_size;
	printf("-------------users: %"PRIu64"---------------\n",sc_user_size);
	for ( i = 0; i<sc_user_size; i++ )
	{
		printf("\tuser i: %"PRIu64"\n",i);
		printf("\tsc_general->scp_user[%"PRIu64"].sc_user->name=%s\n",i,sc_general->scp_user[i].sc_user->name);
		printf("\tsc_general->scp_user[%"PRIu64"].sc_user->passwd=%s\n",i,sc_general->scp_user[i].sc_user->passwd);
		printf("\tsc_general->scp_user[%"PRIu64"].sc_user->encryption_type=%d\n",i,sc_general->scp_user[i].sc_user->encryption_type);
		printf("\tsc_general->scp_user[%"PRIu64"].sc_user->id=%"PRIu64"\n",i,sc_general->scp_user[i].sc_user->id);
	}
	printf("-------------storages: %"PRIu64"---------------\n",sc_storage_size);
	for ( i = 0; i<sc_storage_size; i++ )
	{
		printf("\tstorage i: %"PRIu64"\n",i);
		printf("\tsc_general->scp_storage[%"PRIu64"].sc_storage->name=%s\n",i,sc_general->scp_storage[i].sc_storage->name);
		printf("\tsc_general->scp_storage[%"PRIu64"].sc_storage->type=%s\n",i,sc_general->scp_storage[i].sc_storage->type);
		printf("\tsc_general->scp_storage[%"PRIu64"].sc_storage->type_id=%d\n",i,sc_general->scp_storage[i].sc_storage->type_id);
		printf("\tsc_general->scp_storage[%"PRIu64"].sc_storage->directory=%s\n",i,sc_general->scp_storage[i].sc_storage->directory);
	}
	printf("-------------repositorys: %"PRIu64"---------------\n",sc_repository_size);
	for ( i = 0; i<sc_repository_size; i++ )
	{
		puts("===============================================================");
		printf("\t repository i: %"PRIu64"\n",i);
		printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->name=%s\n",i,sc_general->scp_repository[i].sc_repository->name);
		printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->type=%s\n",i,sc_general->scp_repository[i].sc_repository->type);
		printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->type_id=%d\n",i,sc_general->scp_repository[i].sc_repository->type_id);
		printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->max_version_artifacts=%"PRIu64"\n",i,sc_general->scp_repository[i].sc_repository->max_version_artifacts);
		printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->max_size=%"PRIu64"\n",i,sc_general->scp_repository[i].sc_repository->max_size);
		printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->store_method=%d\n",i,sc_general->scp_repository[i].sc_repository->store_method);
		printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->old_version_repo=%s\n",i,sc_general->scp_repository[i].sc_repository->old_version_repo);
		printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->filesystem=%s\n",i,sc_general->scp_repository[i].sc_repository->filesystem);
		printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->storage=%s\n",i,sc_general->scp_repository[i].sc_repository->storage);
		printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->uri=%s\n",i,sc_general->scp_repository[i].sc_repository->uri);
		printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->uri_size=%"PRIu64"\n",i,sc_general->scp_repository[i].sc_repository->uri_size);
		int64_t sc_script_size = sc_general->scp_repository[i].sc_repository->before_script_size;
		printf("\tmas1(%p)\n",sc_general->scp_repository[i].sc_repository->before_script);
		if ( sc_script_size == -1 )
		{
			printf("-------------shells: onlyshell---------------\n");
			printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->before_script[0].sc_script->scriptpath=%s\n",i,sc_general->scp_repository[i].sc_repository->before_script[0].sc_script->scriptpath);
			printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->before_script[0].sc_script->scriptpath_len=%zu\n",i,sc_general->scp_repository[i].sc_repository->before_script[0].sc_script->scriptpath_len);
		}
		else
		{
			printf("-------------shells: before_script_size %"PRId64"---------------\n",sc_script_size);
			for ( j = 0; j<sc_script_size; j++ )
			{
				printf("\tmas(%p)\n",sc_general->scp_repository[i].sc_repository->before_script);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->before_script[%"PRIu64"].key=%"PRIu64"\n",i,j,sc_general->scp_repository[i].sc_repository->before_script[j].key);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->before_script[%"PRIu64"].value=%s\n",i,j,sc_general->scp_repository[i].sc_repository->before_script[j].value);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->before_script[%"PRIu64"].value_size=%zu\n",i,j,sc_general->scp_repository[i].sc_repository->before_script[j].value_size);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->before_script[%"PRIu64"].match=%"PRIu64"\n",i,j,sc_general->scp_repository[i].sc_repository->before_script[j].match);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->before_script[%"PRIu64"](%p).sc_script(%p)->scriptpath(%p)=%s\n",i,j,&(sc_general->scp_repository[i].sc_repository->before_script[j]),sc_general->scp_repository[i].sc_repository->before_script[j].sc_script,sc_general->scp_repository[i].sc_repository->before_script[j].sc_script->scriptpath,sc_general->scp_repository[i].sc_repository->before_script[j].sc_script->scriptpath);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->before_script[%"PRIu64"].sc_script->scriptpath_len=%zu\n",i,j,sc_general->scp_repository[i].sc_repository->before_script[j].sc_script->scriptpath_len);
			}
		}

		sc_script_size = sc_general->scp_repository[i].sc_repository->between_script_size;
		if ( sc_script_size == -1 )
		{
			printf("-------------shells: onlyshell---------------\n",sc_script_size);
			printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->between_script[0].sc_script->scriptpath=%s\n",i,sc_general->scp_repository[i].sc_repository->between_script[0].sc_script->scriptpath);
			printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->between_script[0].sc_script->scriptpath_len=%zu\n",i,sc_general->scp_repository[i].sc_repository->between_script[0].sc_script->scriptpath_len);
		}
		else
		{
			printf("-------------shells: between_script_size %"PRId64"---------------\n",sc_script_size);
			for ( j = 0; j<sc_script_size; j++ )
			{
				printf("\tmas(%p)\n",sc_general->scp_repository[i].sc_repository->between_script);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->between_script[%"PRIu64"].key=%"PRIu64"\n",i,j,sc_general->scp_repository[i].sc_repository->between_script[j].key);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->between_script[%"PRIu64"].value=%s\n",i,j,sc_general->scp_repository[i].sc_repository->between_script[j].value);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->between_script[%"PRIu64"].value_size=%zu\n",i,j,sc_general->scp_repository[i].sc_repository->between_script[j].value_size);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->between_script[%"PRIu64"].match=%"PRIu64"\n",i,j,sc_general->scp_repository[i].sc_repository->between_script[j].match);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->between_script[%"PRIu64"](%p).sc_script(%p)->scriptpath(%p)=%s\n",i,j,&(sc_general->scp_repository[i].sc_repository->between_script[j]),sc_general->scp_repository[i].sc_repository->between_script[j].sc_script,sc_general->scp_repository[i].sc_repository->between_script[j].sc_script->scriptpath,sc_general->scp_repository[i].sc_repository->between_script[j].sc_script->scriptpath);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->between_script[%"PRIu64"].sc_script->scriptpath_len=%zu\n",i,j,sc_general->scp_repository[i].sc_repository->between_script[j].sc_script->scriptpath_len);
			}
		}
		sc_script_size = sc_general->scp_repository[i].sc_repository->after_script_size;
		if ( sc_script_size == -1 )
		{
			printf("-------------shells: onlyshell---------------\n",sc_script_size);
			printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->after_script[0].sc_script->scriptpath=%s\n",i,sc_general->scp_repository[i].sc_repository->after_script[0].sc_script->scriptpath);
			printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->after_script[0].sc_script->scriptpath_len=%zu\n",i,sc_general->scp_repository[i].sc_repository->after_script[0].sc_script->scriptpath_len);
		}
		else
		{
			printf("-------------shell: after_script_size %"PRId64"---------------\n",sc_script_size);
			for ( j = 0; j<sc_script_size; j++ )
			{
				printf("\tmas(%p)\n",sc_general->scp_repository[i].sc_repository->after_script);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->after_script[%"PRIu64"].key=%"PRIu64"\n",i,j,sc_general->scp_repository[i].sc_repository->after_script[j].key);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->after_script[%"PRIu64"].value=%s\n",i,j,sc_general->scp_repository[i].sc_repository->after_script[j].value);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->after_script[%"PRIu64"].value_size=%zu\n",i,j,sc_general->scp_repository[i].sc_repository->after_script[j].value_size);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->after_script[%"PRIu64"].match=%"PRIu64"\n",i,j,sc_general->scp_repository[i].sc_repository->after_script[j].match);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->after_script[%"PRIu64"](%p).sc_script(%p)->scriptpath(%p)=%s\n",i,j,&(sc_general->scp_repository[i].sc_repository->after_script[j]),sc_general->scp_repository[i].sc_repository->after_script[j].sc_script,sc_general->scp_repository[i].sc_repository->after_script[j].sc_script->scriptpath,sc_general->scp_repository[i].sc_repository->after_script[j].sc_script->scriptpath);
				printf("\tsc_general->scp_repository[%"PRIu64"].sc_repository->after_script[%"PRIu64"].sc_script->scriptpath_len=%zu\n",i,j,sc_general->scp_repository[i].sc_repository->after_script[j].sc_script->scriptpath_len);
			}
		}

	}


}

slarht_conf_general *sc_general_init()
{
	slarht_conf_general *sc_general = calloc(sizeof(slarht_conf_general),1);
	sc_general->config_source=SC_CONFIG_FROM_FILE;
	sc_general->tmpdir=copy_init("/var/slarht");
	sc_general->tmpdir_size=strlen(sc_general->tmpdir);
	sc_general->sc_user_size=-1;
	sc_general->sc_storage_size=-1;
	sc_general->sc_repository_size=-1;
	sc_general->port = 8080;
	sc_general->listen = copy_init_n("127.0.0.1",UCHAR_MAX);
	return sc_general;
}

typedef struct parser_temp
{
	int level;
	int ctx_id;
} parser_temp;

unsigned int parser(slarht_conf_general *sc_general);
void init_prs(FILE **fh, yaml_parser_t *parser);
void parse_next(yaml_parser_t *parser, yaml_event_t *event);
void clean_prs(FILE **fh, yaml_parser_t *parser, yaml_event_t *event);

void event_switch(bool *seq_status, unsigned int *map_seq, slarht_conf_general *sc_general,yaml_parser_t *parser, yaml_event_t *event, parser_temp *ptemp);
void to_data(bool *seq_status, unsigned int *map_seq,  slarht_conf_general *sc_general,yaml_parser_t *parser, yaml_event_t *event,parser_temp *ptemp);
//void parsed_loops(unsigned int *nlp, slarht_conf_general *sc_general);

slarht_conf_general *get_config(char *config_path)
{
	slarht_conf_general *sc_general;
	sc_general = sc_general_init();
	sc_general->config_path = config_path;
	unsigned int nlp;
	nlp = parser(sc_general);
	return sc_general;
}

size_t bytestosize (char *ch)
{
	size_t ch_len = strlen(ch);
	uint64_t i, j, tmp=0;
	for ( i=0; i<ch_len; i++)
	{
		if (ch[i] < 48 ||  ch[i] > 58)
		{
			switch (ch[i])
			{
				case 'k': tmp*=pow(10,3); break;
				case 'm': tmp*=pow(10,6); break;
				case 'g': tmp*=pow(10,9); break;
				case 't': tmp*=pow(10,12); break;
				case 'K': tmp*=pow(2,10); break;
				case 'M': tmp*=pow(2,20); break;
				case 'G': tmp*=pow(2,30); break;
				case 'T': tmp*=pow(2,40); break;
			}
			break;
		}
		tmp*=10;
		tmp+=ch[i]-48;
	}
	return tmp;
}

unsigned int
parser(slarht_conf_general *sc_general)
{
    FILE *fh = fopen(sc_general->config_path,"r");
    yaml_parser_t parser;
    yaml_event_t event;
    
    bool seq_status = 0;
    unsigned int map_seq = 0;

    init_prs(&fh,&parser);
    parser_temp *ptemp = calloc(sizeof(ptemp),1);
    ptemp->level=0;
    ptemp->ctx_id=SC_CONFIG_CTX_GENERAL;
    
    do {
        
        parse_next(&parser,&event);
        
        event_switch(&seq_status,&map_seq,sc_general,&parser,&event,ptemp);   
        
        if ( event.type != YAML_STREAM_END_EVENT ){
            yaml_event_delete(&event);
        }
        
    } while( event.type != YAML_STREAM_END_EVENT );

    sc_general->sc_user_size+=1;
    sc_general->sc_repository_size+=1;
    sc_general->sc_storage_size+=1;
    sc_general_print(sc_general);

    clean_prs(&fh,&parser,&event);
   
    return map_seq;
}

void event_switch(bool *seq_status, unsigned int *map_seq, slarht_conf_general *sc_general,yaml_parser_t *parser, yaml_event_t *event, parser_temp *ptemp)
{
    switch(event->type){
        case YAML_STREAM_START_EVENT: break;
        case YAML_STREAM_END_EVENT: puts("--- stream end"); break;
        case YAML_DOCUMENT_START_EVENT: ptemp->level=-1;break;
        case YAML_DOCUMENT_END_EVENT: break;
        case YAML_SEQUENCE_START_EVENT:
            puts("+++ sequence start");
            ptemp->level+=1;
            (*seq_status) = true;
            break;
        case YAML_SEQUENCE_END_EVENT:
            ptemp->level-=1; break;
            puts("--- sequence end");
            (*seq_status) = false;
            break;
        case YAML_MAPPING_START_EVENT:
            puts("+++ map start");
            ptemp->level+=1;
            if( *seq_status == 1 ){
                (*map_seq)++;
            }
            break;
        case YAML_MAPPING_END_EVENT:
	    puts("--- mapping end");
            ptemp->level-=1; break;
        case YAML_ALIAS_EVENT:
            printf(" ERROR: Got alias (anchor %s)\n", \
                event->data.alias.anchor);
            exit(EXIT_FAILURE);
            break;
        case YAML_SCALAR_EVENT:
            to_data(seq_status,map_seq,sc_general,parser,event,ptemp);
            break;
        case YAML_NO_EVENT:
            puts(" ERROR: No event!");
            exit(EXIT_FAILURE);
            break;
    }
}

void
to_data(bool *seq_status, unsigned int *map_seq, slarht_conf_general *sc_general, yaml_parser_t *parser, yaml_event_t *event, parser_temp *ptemp)
{
	char *buf = (char *) event->data.scalar.value;
	int64_t sh_i;
	printf("test case: [\"%s\"]\tlevel=%d\n",buf,ptemp->level);
	
	if ( ptemp->level == 0 )
	{
		if ( !strcmp(buf, "config") )
		{
			ptemp->level=0;
			parse_next(parser,event);
			if ( !strcmp(event->data.scalar.value, "file") )
				sc_general->config_source = SC_CONFIG_FROM_FILE;
			else
				sc_general->config_source = SC_CONFIG_FROM_FILE;
		}
		else if ( !strcmp(buf, "listen") )
		{
			ptemp->level=0;
			parse_next(parser,event);
			sc_general->listen[0]='\0';
			strncpy(sc_general->listen,event->data.scalar.value,UCHAR_MAX);
		}
		else if ( !strcmp(buf, "port") )
		{
			ptemp->level=0;
			parse_next(parser,event);
			sc_general->port = atoll(event->data.scalar.value);
		}
		else if ( !strcmp(buf,"user") )
		{
			puts("++++++++++++++++++++++++++++++++++++++++++++");
			printf("this is user context\n");
			puts("++++++++++++++++++++++++++++++++++++++++++++");
			parse_next(parser,event);
			ptemp->level=1;
			ptemp->ctx_id=SC_CONFIG_CTX_USER;
			printf("2\n");
		}
		else if ( !strcmp(buf,"repository") )
		{ 
			puts("++++++++++++++++++++++++++++++++++++++++++++");
			printf("this is repository context\n");
			puts("++++++++++++++++++++++++++++++++++++++++++++");
			parse_next(parser,event);
			ptemp->level=1;
			ptemp->ctx_id=SC_CONFIG_CTX_REPOSITORY;
		}
		else if ( !strcmp(buf,"storage") )
		{ 
			puts("++++++++++++++++++++++++++++++++++++++++++++");
			printf("this is storage context\n");
			puts("++++++++++++++++++++++++++++++++++++++++++++");
			ptemp->level=0;
			ptemp->ctx_id=SC_CONFIG_CTX_STORAGE;
		}
		else if ( (*seq_status) == true )
		{
			/* data from sequence of loops */
//////			to_data_from_map(buf,map_seq,coil,parser,event);
			//printf("3\n");
		}
		else
		{
			printf("\n -ERROR: Unknow variable in config file: %s\n",buf);
			exit(EXIT_FAILURE);
		}
	}
	else if ( ptemp->level >= 1 )
	{
		if ( ptemp->ctx_id == SC_CONFIG_CTX_USER )
		{
			printf("this is user context\n");
			printf("%d\n",!strcmp(buf, "name"));
			printf("%d\n",!strcmp(buf, "passwd"));
			if ( !strcmp(buf, "name") )
			{
				//ptemp->level=2;
				sc_general->sc_user_size+=1;
				printf("this is user:name context\n");
				parse_next(parser,event);
				sc_general->scp_user[sc_general->sc_user_size].sc_user = sc_user_init(sc_general->sc_user_size);
				sc_general->scp_user[sc_general->sc_user_size].sc_user->name = copy_init(event->data.scalar.value);
				//printf("sc_general->scp_user[%"PRIu64"].sc_user->name(%p)=%p\n",sc_general->sc_user_size,sc_general->scp_user[sc_general->sc_user_size].sc_user->name, sc_general->scp_user[sc_general->sc_user_size].sc_user->name);
			}
			if ( !strcmp(buf, "passwd") )
			{
				parse_next(parser,event);
				sc_general->scp_user[sc_general->sc_user_size].sc_user->passwd = copy_init(event->data.scalar.value);
				printf("this is user:passwd context\n");
			}
		}
		else if ( ptemp->ctx_id == SC_CONFIG_CTX_REPOSITORY )
		{
			printf("this is repository context\n");
			if ( !strcmp(buf, "name") )
			{
				//ptemp->level=2;
				sc_general->sc_repository_size+=1;
				printf("this is repository:name context\n");
				parse_next(parser,event);
				sc_general->scp_repository[sc_general->sc_repository_size].sc_repository = sc_repository_init(sc_general->sc_repository_size);
				sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->name = copy_init(event->data.scalar.value);
				printf("sc_general->scp_repository[%"PRIu64"].sc_repository->name(%p)=%s\n",sc_general->sc_repository_size,sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->name,sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->name);
			}
			if ( !strcmp(buf, "type") )
			{
				printf("this is repository:type context\n");
				parse_next(parser,event);
				sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->type = copy_init(event->data.scalar.value);
				if ( !strcmp(event->data.scalar.value,"generic") )
					sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->type_id = REPOSITORY_TYPE_GENERIC;
				if ( !strcmp(event->data.scalar.value,"yum") )
					sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->type_id = REPOSITORY_TYPE_YUM;
				if ( !strcmp(event->data.scalar.value,"apt") )
					sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->type_id = REPOSITORY_TYPE_APT;
				if ( !strcmp(event->data.scalar.value,"docker") )
					sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->type_id = REPOSITORY_TYPE_DOCKER;
				
			}
			if ( !strcmp(buf, "max_size") )
			{
				printf("this is repository:max_size context\n");
				parse_next(parser,event);
				sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->max_size = bytestosize(event->data.scalar.value);
			}
			if ( !strcmp(buf, "max_version_artifacts") )
			{
				printf("this is repository:max_version_artifacts context\n");
				parse_next(parser,event);
				sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->max_version_artifacts = bytestosize(event->data.scalar.value);
			}
			if ( !strcmp(buf, "storage") )
			{
				printf("this is repository:storage context\n");
				parse_next(parser,event);
				sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->storage = copy_init(event->data.scalar.value);
				sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->store_method=SLARHT_CONFIG_STORE_STORAGE;
			}
			if ( !strcmp(buf, "filesystem") )
			{
				printf("this is repository:filesystem context\n");
				parse_next(parser,event);
				sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->filesystem = copy_init(event->data.scalar.value);
				sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->store_method=SLARHT_CONFIG_STORE_FILE;
			}
			if ( !strcmp(buf, "old_version_repo") )
			{
				printf("this is repository:old_version_repo context\n");
				parse_next(parser,event);
				sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->old_version_repo = copy_init(event->data.scalar.value);
				sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->old_version_enable=1;
			}
			if ( !strcmp(buf, "uri") )
			{
				printf("this is repository:uri context\n");
				parse_next(parser,event);
				sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->uri = copy_init(event->data.scalar.value);
				sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->uri_size = strlen(sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->uri);
			}
			if ( !strcmp(buf, "shell_before") || !strcmp(buf, "shell_between") || !strcmp(buf, "shell_after") )
			{
				int shell_type;
				//slarht_conf_shell *sc_shell = malloc (sizeof(slarht_conf_shell)*MAX_SCRIPTS_PER_SHELL); // 33333333
				slarht_conf_shell *sc_shell = malloc (sizeof(slarht_conf_shell)); // 33333333
				sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->before_script_context = 0;
				sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->between_script_context = 0;
				sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->after_script_context = 0;
				if ( !strcmp(buf, "shell_before") )
				{
					puts("SHELL_BEFORE");
					shell_type=SC_CONTEXT_SHELL_BEFORE;
				}
				else if ( !strcmp(buf, "shell_between") )
				{
					shell_type=SC_CONTEXT_SHELL_BETWEEN;
				}
				else if ( !strcmp(buf, "shell_after") )
				{
					shell_type=SC_CONTEXT_SHELL_AFTER;
				}
				printf("---------------------------------------\n");
				printf("---------------------------------------\n");
				printf("---------------------------------------\n");
				printf("---------------------------------------\n");
				printf("---------------------------------------\n");
				printf("---------------------------------------\n");
				printf("---------------------------------------\n");
				printf("---------------------------------------\n");
				printf("---------------------------------------\n");
				printf("---------------------------------------\n");
				printf("---------------------------------------\n");
				printf("---------------------------------------\n");
				printf("---------------------------------------\n");
				printf("---------------------------------------\n");
				printf("---------------------------------------\n");
				printf("---------------------------------------\n");
				printf("this is repository:shell context\n");
				//printf("sequence start code is %d\nsequence stop code is %d\nmapping start code is %d\nmapping end code is %d\n",YAML_SEQUENCE_START_EVENT,YAML_SEQUENCE_END_EVENT,YAML_MAPPING_START_EVENT,YAML_MAPPING_END_EVENT);
				int shellonly=0;
				parse_next(parser,event);
				if ( event->type == YAML_SEQUENCE_START_EVENT )
					parse_next(parser,event);
				else
					shellonly=1;
				//if ( event->type != YAML_SCALAR_EVENT )	parse_next(parser,event);

				//parse_next(parser,event);
				//if ( event->type != YAML_SCALAR_EVENT )	parse_next(parser,event);
			//	printf("type%d\n",event->type );
			//	printf("MAPPINGS %d\n",YAML_MAPPING_START_EVENT );
			//	printf("MAPPINGE %d\n",YAML_MAPPING_END_EVENT );
			//	printf("SEQUENCES %d\n",YAML_SEQUENCE_START_EVENT );
			//	printf("SEQUENCEE %d\n",YAML_SEQUENCE_END_EVENT );
				if ( event->type == YAML_MAPPING_START_EVENT )
				{ // for matching logic
					parse_next(parser,event);
					if ( event->type != YAML_SCALAR_EVENT )	parse_next(parser,event);
					printf("1parse_ %s\n",event->data.scalar.value);
					printf("event->data.scalar.value=%s\n",event->data.scalar.value);
					for ( sh_i=0; event->type != YAML_SEQUENCE_END_EVENT; sh_i++, parse_next(parser,event) )
					{
						if ( event->type == YAML_MAPPING_START_EVENT )
						{
							parse_next(parser,event);
							sc_shell = realloc (sc_shell,sh_i*sizeof(slarht_conf_shell)); // 33333333
						}
						puts("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
						sc_shell[sh_i].sc_script = malloc (sizeof(slarht_conf_script));
						int first = 1;
						do //while ( event->type == YAML_SEQUENCE_END_EVENT )
						{
							if ( first == 0 )	parse_next(parser, event);
							first = 0;
							printf("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&%"PRIu64"&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n", sh_i);
							if ( event->type == 6 ) printf("0parse_ %s\n",event->data.scalar.value);
							else			printf("0type %d\n",event->type );
							if ( event->type == YAML_MAPPING_END_EVENT ) break;
							if ( !strcmp(event->data.scalar.value, "key") )
							{
								puts("\tkey context");
								parse_next(parser,event);
								if ( event->type == 6 ) printf("2parse_ %s\n",event->data.scalar.value);
								else			printf("2type %d\n",event->type );
								if ( !strcmp(event->data.scalar.value, "filename") )
								{
									sc_shell[sh_i].key=SC_VARIABLE_FILENAME;
								}
								else if ( !strcmp(event->data.scalar.value, "filepath") )
								{
									sc_shell[sh_i].key=SC_VARIABLE_FILEPATH;
								}
								else if ( !strcmp(event->data.scalar.value, "dirname") )
								{
									sc_shell[sh_i].key=SC_VARIABLE_DIRNAME;
								}
							}
							else if ( !strcmp(event->data.scalar.value, "script") )
							{
								puts("\tscript context");
								char *scriptfile = malloc(FILENAME_MAX);
								char *scriptdir = malloc(FILENAME_MAX);
								struct timespec now_time;
								clock_gettime(CLOCK_REALTIME, &now_time);
								snprintf(scriptdir, FILENAME_MAX-1, "%s/scr%lld.%09ld",sc_general->tmpdir,now_time.tv_sec,now_time.tv_nsec);
								snprintf(scriptfile, FILENAME_MAX-1, "%s/%"PRIu64".sh",scriptdir,sh_i);
								size_t scriptfilename_size = strlen(scriptfile);
								mkdirp(scriptdir);
								FILE *fd_script = fopen(scriptfile, "w");
								parse_next(parser,event);
								if ( event->type == YAML_SEQUENCE_START_EVENT ) parse_next(parser,event);
								if ( event->type == 6 ) printf("3parse_ %s\n",event->data.scalar.value);
								else			printf("3type %d\n",event->type );
								sc_shell[sh_i].sc_script = malloc (sizeof(slarht_conf_script));
								while ( event->type == YAML_SCALAR_EVENT )
								{
									fprintf(fd_script,"%s\n",event->data.scalar.value);
									parse_next(parser,event);
									if ( event->type == 6 ) printf("8parse_ %s\n",event->data.scalar.value);
									else			printf("8type %d\n",event->type );
								}
								sc_shell[sh_i].sc_script->scriptpath = scriptfile;
								sc_shell[sh_i].sc_script->scriptpath_len = strlen(scriptfile);
								fclose(fd_script);
								chmod(scriptfile,755);
								event->type = 250;
								if ( event->type == 6 ) printf("4parse_ %s\n",event->data.scalar.value);
								else			printf("4type %d\n",event->type );
							}
							else if ( !strcmp(event->data.scalar.value, "value") )
							{
								puts("\tvalue context");
								parse_next(parser,event);
								if ( event->type == 6 ) printf("5parse_ %s\n",event->data.scalar.value);
								else			printf("5type %d\n",event->type );
								sc_shell[sh_i].value=copy_init(event->data.scalar.value);
								sc_shell[sh_i].value_size = strlen(event->data.scalar.value);
							}
							else if ( !strcmp(event->data.scalar.value, "match") )
							{
								puts("\tmatch context");
								parse_next(parser,event);
								if ( event->type == 6 ) printf("6parse_ %s\n",event->data.scalar.value);
								else			printf("6type %d\n",event->type );
								if ( !strcmp(event->data.scalar.value, "startswith") )
								{
									sc_shell[sh_i].match = SC_REPOSITORY_SHELL_MATCH_STARTSWITH;
								}
								else if ( !strcmp(event->data.scalar.value, "endswith") )
								{
									sc_shell[sh_i].match = SC_REPOSITORY_SHELL_MATCH_ENDSWITH;
								}
								else if ( !strcmp(event->data.scalar.value, "has") )
								{
									sc_shell[sh_i].match = SC_REPOSITORY_SHELL_MATCH_HAS;
								}
								else if ( !strcmp(event->data.scalar.value, "regexp") )
								{
									sc_shell[sh_i].match = SC_REPOSITORY_SHELL_MATCH_REGEXP;
								}
							}
							else puts("else context");
						} while ( event->type != YAML_MAPPING_END_EVENT );
					}
					if ( shellonly == 1 )
					{
						sc_shell[sh_i].value_size=0;
						sc_shell[sh_i].match=0;
						if (shell_type==SC_CONTEXT_SHELL_BEFORE)
						{
							sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->before_script_size = -1;
						}
						else if (shell_type==SC_CONTEXT_SHELL_BETWEEN)
						{
							sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->between_script_size = -1;
						}
						else if (shell_type==SC_CONTEXT_SHELL_AFTER)
						{
							puts("4SHELL_AFTER");
							sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->after_script_size = -1;
						}
					}
					else
					{
						if (shell_type==SC_CONTEXT_SHELL_BEFORE)
						{
							puts("2SHELL_BEFORE");
							sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->before_script_size = sh_i;
							printf("before size is %"PRIu64"\n", sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->before_script_size);
						}
						else if (shell_type==SC_CONTEXT_SHELL_BETWEEN)
						{
							sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->between_script_size = sh_i;
						}
						else if (shell_type==SC_CONTEXT_SHELL_AFTER)
						{
							puts("2SHELL_AFTER");
							sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->after_script_size = sh_i;
						}
					}
				}
				//else 
				//{
					//puts("\tsimply script context");
					//char *scriptfile = malloc(FILENAME_MAX);
					//char *scriptdir = malloc(FILENAME_MAX);
					//struct timespec now_time;
					//clock_gettime(CLOCK_REALTIME, &now_time);
					//snprintf(scriptdir, FILENAME_MAX-1, "%s/scr%lld.%09ld",sc_general->tmpdir,now_time.tv_sec,now_time.tv_nsec);
					//snprintf(scriptfile, FILENAME_MAX-1, "%s/%"PRIu64".sh",scriptdir,sh_i);
					//size_t scriptfilename_size = strlen(scriptfile);
					//mkdirp(scriptdir);
					//FILE *fd_script = fopen(scriptfile, "w");
					//parse_next(parser,event);
					//if ( event->type == YAML_SEQUENCE_START_EVENT ) parse_next(parser,event);
					//if ( event->type == 6 ) printf("3parse_ %s\n",event->data.scalar.value);
					//else			printf("3type %d\n",event->type );
					//sc_shell[sh_i].sc_script = malloc (sizeof(slarht_conf_script));
					//while ( event->type == YAML_SCALAR_EVENT )
					//{
					//	fprintf(fd_script,"%s\n",event->data.scalar.value);
					//	parse_next(parser,event);
					//	if ( event->type == 6 ) printf("8parse_ %s\n",event->data.scalar.value);
					//	else			printf("8type %d\n",event->type );
					//}
					//sc_shell[sh_i].sc_script->scriptpath = scriptfile;
					//sc_shell[sh_i].sc_script->scriptpath_len = strlen(scriptfile);
					//fclose(fd_script);
					//event->type = 250;

					//sc_shell[sh_i].key = 0;
					//sc_shell[sh_i].value_size = 0;
					//if ( event->type == 6 ) printf("4parse_ %s\n",event->data.scalar.value);
					//else			printf("4type %d\n",event->type );
					//if (shell_type==SC_CONTEXT_SHELL_BEFORE)
					//{
					//	puts("5SHELL_BETFORE");
					//	sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->before_script_size = -1;
					//}
					//else if (shell_type==SC_CONTEXT_SHELL_BETWEEN)
					//{
					//	puts("5SHELL_BETWEEN");
					//	sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->between_script_size = -1;
					//}
					//else if (shell_type==SC_CONTEXT_SHELL_AFTER)
					//{
					//	puts("5SHELL_AFTER");
					//	sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->after_script_size = -1;
					//}
				//}
				if (shell_type==SC_CONTEXT_SHELL_BEFORE)
				{
					puts("3SHELL_BEFORE");
					sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->before_script_context = 1;
					sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->before_script = sc_shell;
				}
				else if (shell_type==SC_CONTEXT_SHELL_BETWEEN)
				{
					puts("3SHELL_BETWEEN");
					sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->between_script_context = 1;
					sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->between_script = sc_shell;
				}
				else if (shell_type==SC_CONTEXT_SHELL_AFTER)
				{
					puts("3SHELL_AFTER");
					sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->after_script_context = 1;
					sc_general->scp_repository[sc_general->sc_repository_size].sc_repository->after_script = sc_shell;
				}
			}



		}

		else if ( ptemp->ctx_id == SC_CONFIG_CTX_STORAGE )
		{
			printf("this is storage context\n");
			if ( !strcmp(buf, "name") )
			{
				//ptemp->level=2;
				sc_general->sc_storage_size+=1;
				printf("storagesize=%"PRIu64"\n",sc_general->sc_storage_size);
				printf("this is storage:name context\n");
				parse_next(parser,event);
				sc_general->scp_storage[sc_general->sc_storage_size].sc_storage = sc_storage_init(sc_general->sc_storage_size);
				sc_general->scp_storage[sc_general->sc_storage_size].sc_storage->name = copy_init(event->data.scalar.value);
			}
			if ( !strcmp(buf, "type") )
			{
				printf("this is storage:type context\n");
				parse_next(parser,event);
				sc_general->scp_storage[sc_general->sc_storage_size].sc_storage->type = copy_init(event->data.scalar.value);
				if ( event->data.scalar.value == STORAGE_ON_FS )
					sc_general->scp_storage[sc_general->sc_storage_size].sc_storage->type_id=STORAGE_ON_FS;
			}
			if ( !strcmp(buf, "directory") )
			{
				printf("this is storage:directory context\n");
				parse_next(parser,event);
				sc_general->scp_storage[sc_general->sc_storage_size].sc_storage->directory = copy_init(event->data.scalar.value);
			}
		}
	}
	printf("leaved with level=%d\n",ptemp->level);
}

void
parse_next(yaml_parser_t *parser, yaml_event_t *event)
{
	if ( !yaml_parser_parse(parser, event) ){
		printf("Parser error %d\n", parser->error);
		exit(EXIT_FAILURE);
	}
}
void
init_prs(FILE ** fh, yaml_parser_t * parser)
{
	/* parser initilization */
	if( !yaml_parser_initialize(parser) ){
		fputs("Failed to initialize parser!\n",stderr);
	}
	if( *fh == NULL ){
		fputs("Failed to open file!\n",stderr);
	}

	yaml_parser_set_input_file(parser, *fh); /* set input file */
}

void
clean_prs(FILE ** fh, yaml_parser_t * parser, yaml_event_t * event)
{
	yaml_event_delete(event);
	yaml_parser_delete(parser);
	fclose(*fh);
}
