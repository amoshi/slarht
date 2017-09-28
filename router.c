#include <stddef.h>
#include <inttypes.h>
#include <string.h>
#include "config_yaml.h"
#include "evhttp.h"

size_t startswith_matches(char *str, char *match)
{
	uint64_t i;
	size_t n = strlen ( match );
	for ( i=0; i<n && str[i]==match[i]; i++ );
	return i;
}

size_t startswith_matches_n(char *str, char *match, size_t n)
{
	uint64_t i;
	for ( i=0; i<n && str[i]==match[i]; i++ );
	return i;
}

slarht_conf_repository *route_get_repository(slarht_conf_general *sc_general, char *query)
{
	uint64_t i, max = 0, rc;
	slarht_conf_repository *ret;
	size_t sc_repository_size = sc_general->sc_repository_size;
	for ( i=0; i<sc_repository_size; i++ )
	{
		rc = startswith_matches_n(query, sc_general->scp_repository[i].sc_repository->uri, sc_general->scp_repository[i].sc_repository->uri_size );
		printf("matching: result %zu: %s and %s\n",rc,query, sc_general->scp_repository[i].sc_repository->uri);
		if ( rc > max )
		{
			max=rc;
			ret=sc_general->scp_repository[i].sc_repository;
		}
	}
	return ret;
}

route_resolver(slarht_conf_general *sc_general, http_traf *ht)
{
	uint64_t i, max = 0, rc;
	size_t sc_repository_size = sc_general->sc_repository_size;
	for ( i=0; i<sc_repository_size; i++ )
	{
		rc = startswith_matches_n(ht->query, sc_general->scp_repository[i].sc_repository->uri, sc_general->scp_repository[i].sc_repository->uri_size );
		printf("matching: result %zu: %s and %s\n",rc,ht->query, sc_general->scp_repository[i].sc_repository->uri);
		if ( rc > max )
		{
			max=rc;
			ht->sc_repository=sc_general->scp_repository[i].sc_repository;
		}
	}
}