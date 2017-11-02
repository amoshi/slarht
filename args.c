void get_http_args()
{
	struct evkeyval *http_arg;
	struct evkeyvalq *http_args;
	char *ptrtoargs = strstr(http_uri,"?");
	http_args = malloc (sizeof(struct evkeyval)*50);
	for (i=0, http_arg = http_args->tqh_first; http_arg; http_arg = http_arg->next.tqe_next, i++)
	{
		printf("  %s: %s\n", http_arg->key, http_arg->value);
	}
	ht->args_len = i;
	ht->args = malloc(sizeof(http_kv)*ht->args_len);
	for (i=0, http_arg = http_args->tqh_first; http_arg; http_arg = http_arg->next.tqe_next, i++)
	{
		ht->args[i].key = http_arg->key;
		ht->args[i].value = http_arg->value;
		printf("COPYING %s and %s\n",ht->args[i].key,ht->args[i].value);
		printf("  %s: %s\n", http_arg->key, http_arg->value);
	}
}
