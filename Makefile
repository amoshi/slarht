all:
	mkdir caches
	cc -c mkdirp.c -o caches/mkdirp.o
	cc -c config_yaml.c -lyaml -o caches/config_yaml.o
	cc -c artifacts.c -lconfig_yaml -Lcaches/ -o caches/artifacts.o
	cc -c rpmburner.c -lconfig_yaml -lartifacts -Lcaches/ -o caches/rpmburner.o
	cc -c router.c -lconfig_yaml -lartifacts -Lcaches/ -o caches/router.o
	cc -c genericrepo.c -lconfig_yaml -lartifacts -Lcaches/ -o caches/genericrepo.o
	cc -c debburner.c -lconfig_yaml -lartifacts -Lcaches/ -o caches/debburner.o
	cc -c repcheck.c -lconfig_yaml -lartifacts -Lcaches/ -o caches/repcheck.o
	cc -c repoburner.c -lconfig_yaml -lartifacts -Lcaches/ -o caches/repoburner.o
	ar rcs caches/librpmburner.a caches/rpmburner.o
	ar rcs caches/libconfig_yaml.a caches/config_yaml.o
	ar rcs caches/libmkdirp.a caches/mkdirp.o
	ar rcs caches/libartifacts.a caches/artifacts.o
	ar rcs caches/librouter.a caches/router.o
	ar rcs caches/libgenericrepo.a caches/genericrepo.o
	ar rcs caches/libdebburner.a caches/debburner.o
	ar rcs caches/librepcheck.a caches/repcheck.o
	ar rcs caches/librepoburner.a caches/repoburner.o
	cc evhttp.c -levent -lconfig_yaml -lrpmburner -lrepoburner -lartifacts -lyaml -lrouter -lgenericrepo -ldebburner -lrepcheck -lmkdirp -Lcaches/ -o caches/slarht
clean:
	rm -rf caches/
