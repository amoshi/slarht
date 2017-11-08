repo_conf* deb_conf(http_traf *ht)
{
	repo_conf *rconf = malloc(sizeof(repo_conf));
	char *command = malloc(SHRT_MAX);
	char *prefix_repository_url = malloc(FILENAME_MAX);
	//size_t command_size = strlen(command);
	char *ns;
	size_t nst;
	char packagename[FILENAME_MAX];
	char mainconst[] = "main";
	char failformat[] = "filename has incorrect format";
	char failpost[] = "filename not ending with '.deb'";
	char *architecture = ht->pkg_architecture ? ht->pkg_architecture : NULL;
	char *distribution = ht->pkg_distribution ? ht->pkg_distribution : mainconst;
	size_t namesize;
	ns = strstr(ht->filename, ".deb");
	if ( ns == NULL )
	{
		ht->error_message = failpost;
		return NULL;
	}
	nst = strcspn(ht->filename, "_-");
	if ( nst == 0 )
	{
		ht->error_message = failformat;
		return NULL;
	}
	namesize = nst;
	printf("namesize=%zu\n",namesize);
	if ( architecture == NULL )
		snprintf(command,SHRT_MAX,"reprepro --silent --ignore=extension -b '%s' -C %s includedeb general '%s'",ht->sc_repository->filesystem, distribution, ht->file_cache_path);
	else
		snprintf(command,SHRT_MAX,"reprepro --silent --ignore=extension -b '%s' -C %s -A %s includedeb general '%s'",ht->sc_repository->filesystem, distribution, architecture, ht->file_cache_path);
	printf("strncpy(%zu);\n",namesize);
	printf("strncpy(%p);\n", ht->filename);
	printf("strncpy(%p);\n",packagename);
	strncpy(packagename, ht->filename, namesize);
	packagename[namesize]='\0';
	printf("%s\n",distribution);
	printf("%c\n",ht->filename[0]);
	printf("%s\n",packagename);
	printf("%s\n",ht->filename);
	snprintf(prefix_repository_url,FILENAME_MAX-1,"pool/%s/%c/%s", distribution, ht->filename[0], packagename);
	printf("prefix=%s\n",prefix_repository_url);

	rconf->deploy_method=DEPLOY_METHOD_SCRIPT;
	rconf->deploy_method_post=DEPLOY_METHOD_POST_NO;
	rconf->command=command;
	rconf->ht=ht;
	ht->prefix_repository_url = prefix_repository_url;
	return rconf;
}
