#pragma once
#define MAX_USERNAME_SIZE 255
#define MAX_PASSWD_SIZE 255
#define SC_CONFIG_FROM_FILE 0
#define SC_CONFIG_CTX_GENERAL 0
#define SC_CONFIG_CTX_USER 1
#define SC_CONFIG_CTX_STORAGE 2
#define SC_CONFIG_CTX_REPOSITORY 3
#define SLARHT_CONFIG_STORE_FILE 0
#define SLARHT_CONFIG_STORE_STORAGE 1
#define MAX_USER_SIZE 1000
#define MAX_REPOSITORY_SIZE 1000
#define MAX_STORAGE_SIZE 1000
#define REPOSITORY_TYPE_GENERIC 0
#define REPOSITORY_TYPE_YUM 1
#define REPOSITORY_TYPE_APT 2
#define REPOSITORY_TYPE_DOCKER 3
#define STORAGE_ON_FS 0
#define SC_CONFIG_CTX_REPOSITORY_SHELL_BEFORE 1
#define SC_VARIABLE_FILEPATH 1
#define SC_VARIABLE_FILENAME 2
#define SC_VARIABLE_DIRNAME 3
#define SC_REPOSITORY_SHELL_MATCH_STARTSWITH 0
#define SC_REPOSITORY_SHELL_MATCH_ENDSWITH 1
#define SC_REPOSITORY_SHELL_MATCH_HAS 1
#define SC_REPOSITORY_SHELL_MATCH_REGEXP 1
#define SC_CONTEXT_SHELL_BEFORE 0
#define SC_CONTEXT_SHELL_BETWEEN 1
#define SC_CONTEXT_SHELL_AFTER 2

typedef struct slarht_conf_user
{
	uint64_t id;
	char *name;
	char *passwd;
	int encryption_type;
} slarht_conf_user;

typedef struct slarht_conf_storage
{
	char *name;
	char *type;
	int type_id;
	uint64_t id;
	char *directory;
} slarht_conf_storage;

typedef struct slarht_conf_script
{
	char *scriptpath;
	size_t scriptpath_len;
} slarht_conf_script;

typedef struct slarht_conf_shell
{
	slarht_conf_script *sc_script;
	uint64_t key;
	uint64_t match;
	char *value;
	size_t value_size;
} slarht_conf_shell;

typedef struct slarht_conf_repository
{
	uint64_t id;
	char *name;
	char *uri;
	size_t uri_size;
	char *filesystem;
	int type_id;
	char *type;
	uint64_t max_size;
	int store_method;
	int old_version_enable;
	char *old_version_repo;
	uint64_t max_version_artifacts;
	char *storage;
	slarht_conf_storage *storage_ptr;
	char *acl_ro_network;
	char *acl_network;
	uint64_t *acl_user;
	uint64_t *acl_ro_user;
	int network_ro_vis;
	int user_ro_vis;
	int before_script_context;
	int between_script_context;
	int after_script_context;
	slarht_conf_shell *before_script;
	size_t before_script_size;
	slarht_conf_shell *between_script;
	size_t between_script_size;
	slarht_conf_shell *after_script;
	size_t after_script_size;
} slarht_conf_repository;

typedef struct slarth_conf_ptr_user {
	slarht_conf_user *sc_user;
} slarth_conf_ptr_user;

typedef struct slarth_conf_ptr_repository {
	slarht_conf_repository *sc_repository;
} slarth_conf_ptr_repository;

typedef struct slarth_conf_ptr_storage {
	slarht_conf_storage *sc_storage;
} slarth_conf_ptr_storage;

typedef struct slarht_conf_general
{
	int config_source;
	int tmpdir_size;
	char *listen;
	int64_t port;
	char *config_path;
	char *tmpdir;
	slarth_conf_ptr_user scp_user[MAX_USER_SIZE];
	slarth_conf_ptr_repository scp_repository[MAX_REPOSITORY_SIZE];
	slarth_conf_ptr_storage scp_storage[MAX_STORAGE_SIZE];
	size_t sc_user_size;
	size_t sc_repository_size;
	size_t sc_storage_size;
} slarht_conf_general;
