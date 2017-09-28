#pragma once
#include <stddef.h>
#include <inttypes.h>
#include "config_yaml.h"
#include "evhttp.h"
typedef struct wrdata
{
        char *data;
        char *dirname;
        char *filename;
        size_t data_size;
        slarht_conf_repository *sc_repository;
} wrdata;
typedef struct exdata
{
        char *command;
} exdata;
