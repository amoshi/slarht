#include <stdio.h>
#include <yaml.h>
#include <stdbool.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
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
	uint64_t i;
	uint64_t sc_user_size = sc_general->sc_user_size;
	uint64_t sc_storage_size = sc_general->sc_storage_size;
	uint64_t sc_repository_size = sc_general->sc_repository_size;
	printf("-------------users: %"PRIu64"---------------\n",sc_user_size);
	for ( i = 0; i<sc_user_size; i++ )
	{
		printf("\ti: %"PRIu64"\n",i);
		printf("\tsc_general->scp_user[%"PRIu64"].sc_user->name=%s\n",i,sc_general->scp_user[i].sc_user->name);
		printf("\tsc_general->scp_user[%"PRIu64"].sc_user->passwd=%s\n",i,sc_general->scp_user[i].sc_user->passwd);
		printf("\tsc_general->scp_user[%"PRIu64"].sc_user->encryption_type=%d\n",i,sc_general->scp_user[i].sc_user->encryption_type);
		printf("\tsc_general->scp_user[%"PRIu64"].sc_user->id=%"PRIu64"\n",i,sc_general->scp_user[i].sc_user->id);
	}
	printf("-------------storages: %"PRIu64"---------------\n",sc_storage_size);
	for ( i = 0; i<sc_storage_size; i++ )
	{
		printf("\ti: %"PRIu64"\n",i);
		printf("\tsc_general->scp_storage[%"PRIu64"].sc_storage->name=%s\n",i,sc_general->scp_storage[i].sc_storage->name);
		printf("\tsc_general->scp_storage[%"PRIu64"].sc_storage->type=%s\n",i,sc_general->scp_storage[i].sc_storage->type);
		printf("\tsc_general->scp_storage[%"PRIu64"].sc_storage->type_id=%d\n",i,sc_general->scp_storage[i].sc_storage->type_id);
		printf("\tsc_general->scp_storage[%"PRIu64"].sc_storage->directory=%s\n",i,sc_general->scp_storage[i].sc_storage->directory);
	}
	printf("-------------repositorys: %"PRIu64"---------------\n",sc_repository_size);
	for ( i = 0; i<sc_repository_size; i++ )
	{
		printf("\ti: %"PRIu64"\n",i);
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
	}


}

slarht_conf_general *sc_general_init()
{
	slarht_conf_general *sc_general = calloc(sizeof(slarht_conf_general),1);
	sc_general->config_source=SC_CONFIG_FROM_FILE;
	sc_general->tmpdir=copy_init("/var/slarht");
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
