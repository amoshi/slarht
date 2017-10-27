repo_conf* generic_conf(http_traf *ht)
{
	repo_conf *rconf = malloc(sizeof(repo_conf));
	size_t command_size = 0;

	rconf->deploy_method=DEPLOY_METHOD_NO;
	rconf->deploy_method_post=DEPLOY_METHOD_POST_NO;
	rconf->command=NULL;
	rconf->ht=ht;
	return rconf;
}
