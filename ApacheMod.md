## Compile DSO ##
### install apache ###
```
    #cd httpd-2.0.63
    #./configure --prefix=/usr/local/apache2 --enable-rewrite --enable-so --enable-cgi    
    #make build; make install;
    
    --restore apache config from back up
    --find a last backup in BACKSERVER:/opt/archive_backup/DATE/apache_config.tar.gz
    --Extract apache_config.tar.gz to /usr/local/apache2/conf
```
### install DSO module ###
```
    #/usr/local/apache2/bin/apxs -i -a -c proxy.c    
    #/usr/local/apache2/bin/apxs -i -a -c mod_proxy.c proxy_util.c
    #/usr/local/apache2/bin/apxs -i -a -c proxy_connect.c
    #/usr/local/apache2/bin/apxs -i -a -c proxy_http.c
```

## Fast CGI ##
#apxs -o mod\_fastcgi.so -c **.c**

> fcgi\_config.c: In function 'fcgi\_config\_new\_static\_server':
fcgi\_config.c:769: warning: format '%ld' expects type 'long int', but argument 3 has type 'gid\_t'
fcgi\_config.c:774: warning: format '%ld' expects type 'long int', but argument 3 has type 'uid\_t'
fcgi\_config.c: In function 'fcgi\_config\_new\_external\_server':
fcgi\_config.c:962: warning: format '%ld' expects type 'long int', but argument 3 has type 'gid\_t'
fcgi\_config.c:967: warning: format '%ld' expects type 'long int', but argument 3 has type 'uid\_t'

vi fcgi\_config.c change '%ld' to '%ho'

  1. pxs -i -a -n fastcgi mod\_fastcgi.la