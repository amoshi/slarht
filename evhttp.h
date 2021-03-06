#pragma once
#include <stdlib.h>
#include <evhttp.h>
#include "config_yaml.h"
#define REQ_HEAD EVHTTP_REQ_HEAD
#define REQ_GET EVHTTP_REQ_GET
#define REQ_POST EVHTTP_REQ_POST
#define REQ_PUT EVHTTP_REQ_PUT
#define REQ_OPTIONS EVHTTP_REQ_OPTIONS
#define REQ_CONNECT EVHTTP_REQ_CONNECT
#define REQ_DELETE EVHTTP_REQ_DELETE
#define REQ_TRACE EVHTTP_REQ_TRACE
#define REQ_PATCH EVHTTP_REQ_PATCH
#define HTTP_QUERY_MAXSIZE 2048
#define HTTP_DATA_MAXSIZE 10737418240
#define HTTP_HEADERS_MAXCOUNT 400
#define HTTP_ARGS_MAXCOUNT 400
#define HTTP_METHOD_CHAR_MAXSIZE 8
#define DEPLOY_METHOD_NO 0
#define DEPLOY_METHOD_SCRIPT 1
#define DEPLOY_METHOD_PROXYING 2
#define DEPLOY_METHOD_PUSHTODIR 3
#define DEPLOY_METHOD_POST_NO 0
#define DEPLOY_METHOD_POST_FUNC 1

typedef struct http_kv
{
	char *key;
	size_t key_size;
	char *value;
	size_t value_size;
} http_kv;

typedef struct http_traf
{
	int method_id;
	char *method;
	uint64_t method_size;
	char *query;
	uint64_t query_size;
	char *filename;
	int filename_size;
	char *dirname;
	int dirname_size;
	char *filepath;
	int filepath_size;
	const char *full_uri;
	uint64_t full_uri_size;
	char *data;
	char *file_cache_path;
	char *error_message;
	char *prefix_repository_url;
	uint64_t data_size;
	http_kv *args;
	http_kv *headers;
	uint64_t args_len;
	uint64_t headers_len;
	char *host;
	uint64_t host_size;
	char *downloaduri;
	uint64_t downloaduri_size;
	char *_template;
	uint64_t _template_size;
	char *pkg_distribution;
	uint64_t pkg_distribution_size;
	char *pkg_architecture;
	uint64_t pkg_architecture_size;
	char *content_type;
	uint64_t content_type_size;
	slarht_conf_repository *sc_repository;
	char* (*index_generator)(void*, void*, void*);
	int dir_to_index;
	char *authtoken;
	size_t authtoken_size;
	char *http_basic_hash;
	size_t http_basic_hash_size;
	int auth;
} http_traf;

typedef struct repo_conf
{
	char *command;
	size_t command_size;
	int deploy_method;
	int deploy_method_post;
	http_traf *ht;
	void (*func)(void*, void*);
	void *arg;
	void *arg2;
} repo_conf;
