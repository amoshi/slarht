repo_conf* rpm_conf(http_traf *ht)
{
	repo_conf *rconf = malloc(sizeof(repo_conf));
	char *command = malloc(SHRT_MAX);
	size_t command_size = strlen(command);
	snprintf(command,SHRT_MAX-1,"createrepo --update %s",ht->sc_repository->filesystem);

	rconf->deploy_method=DEPLOY_METHOD_SCRIPT;
	rconf->deploy_method_post=DEPLOY_METHOD_POST_NO;
	rconf->command=command;
	rconf->ht=ht;
	return rconf;
}
