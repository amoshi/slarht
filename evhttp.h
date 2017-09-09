#pragma once
#include <evhttp.h>
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

typedef struct http_kv
{
        char *key;
        char *value;
} http_kv;

typedef struct http_traf
{
        int method_id;
        char *method;
        uint64_t method_size;
        char *query;
        uint64_t query_size;
        char *full_uri;
        uint64_t full_uri_size;
        char *data;
        uint64_t data_size;
        http_kv *args;
        http_kv *headers;
        uint64_t args_len;
        uint64_t headers_len;
} http_traf;

http_traf *http_traf_initialize(uint64_t size)
{
	http_traf *ht;
	ht->method_id=0;
	ht->method_size=0;
	ht->query = malloc(HTTP_QUERY_MAXSIZE);
	ht->query_size = 0;
	//ht->data = malloc(HTTP_QUERY_MAXSIZE);
	ht->data_size=0;
	ht->args = malloc(sizeof(http_kv)*HTTP_ARGS_MAXCOUNT);
	ht->headers = malloc(sizeof(http_kv)*HTTP_ARGS_MAXCOUNT);
	ht->headers_len = 0;
	ht->args_len = 0;

	return ht;
}

char *alloc_and_copy_n(char *data, size_t len)
{
	len++;
	char *s = malloc(len);
	snprintf(s,len,"%s",data);
	return s;
}
