# Install subversion #
bzip2 -d subversion-1.5.4.tar.bz2
tar xvf subversion-1.5.4.tar

./configure --prefix=/usr/local/subversion1.5.4
config.status: executing mkdir-init commands
configure: WARNING: we have configured without BDB filesystem support


You don't seem to have Berkeley DB version 4.0.14 or newer
installed and linked to APR-UTIL.  We have created Makefiles which
will build without the Berkeley DB back-end; your repositories will
use FSFS as the default back-end.  You can find the latest version of
Berkeley DB here:
> http://www.oracle.com/technology/software/products/berkeley-db/index.html


make
/bin/bash /opt/downloads/subversion-1.5.4/libtool --tag=CC --silent --mode=link gcc  -g -O2   -pthreads   -rpath /usr/local/subversion1.5.4/lib -o svn  add-cmd.o blame-cmd.o cat-cmd.o changelist-cmd.o checkout-cmd.o cleanup-cmd.o commit-cmd.o conflict-callbacks.o copy-cmd.o delete-cmd.o diff-cmd.o export-cmd.o help-cmd.o import-cmd.o info-cmd.o list-cmd.o lock-cmd.o log-cmd.o main.o merge-cmd.o mergeinfo-cmd.o mkdir-cmd.o move-cmd.o notify.o propdel-cmd.o propedit-cmd.o propget-cmd.o proplist-cmd.o props.o propset-cmd.o resolve-cmd.o resolved-cmd.o revert-cmd.o status-cmd.o status.o switch-cmd.o unlock-cmd.o update-cmd.o util.o ../../subversion/libsvn\_client/libsvn\_client-1.la ../../subversion/libsvn\_wc/libsvn\_wc-1.la ../../subversion/libsvn\_ra/libsvn\_ra-1.la ../../subversion/libsvn\_delta/libsvn\_delta-1.la ../../subversion/libsvn\_diff/libsvn\_diff-1.la ../../subversion/libsvn\_subr/libsvn\_subr-1.la /usr/local/apache2/lib/libaprutil-1.la -lexpat -liconv /usr/local/apache2/lib/libapr-1.la -lrt -lsocket -lnsl  -lpthread -ldl  -lsocket -lintl
ld: warning: file ../../subversion/libsvn\_ra/.libs/libsvn\_ra-1.so: linked to /opt/downloads/subversion-1.5.4/subversion/libsvn\_ra/.libs/libsvn\_ra-1.so: attempted multiple inclusion of file
ld: warning: file ../../subversion/libsvn\_delta/.libs/libsvn\_delta-1.so: linked to /opt/downloads/subversion-1.5.4/subversion/libsvn\_delta/.libs/libsvn\_delta-1.so: attempted multiple inclusion of file
ld: warning: file ../../subversion/libsvn\_diff/.libs/libsvn\_diff-1.so: linked to /opt/downloads/subversion-1.5.4/subversion/libsvn\_diff/.libs/libsvn\_diff-1.so: attempted multiple inclusion of file
ld: warning: file ../../subversion/libsvn\_subr/.libs/libsvn\_subr-1.so: linked to /opt/downloads/subversion-1.5.4/subversion/libsvn\_subr/.libs/libsvn\_subr-1.so: attempted multiple inclusion of file
Undefined                       first referenced
> symbol                             in file
libintl\_bind\_textdomain\_codeset     /opt/downloads/subversion-1.5.4/subversion/libsvn\_subr/.libs/libsvn\_subr-1.so
libintl\_bindtextdomain              /opt/downloads/subversion-1.5.4/subversion/libsvn\_subr/.libs/libsvn\_subr-1.so
libintl\_dgettext                    blame-cmd.o
ld: fatal: Symbol referencing errors. No output written to .libs/svn
collect2: ld returned 1 exit status
make: **[subversion/svn/svn] Error 1**

#Add the line LIBS="$LIBS -lintl" to the configure file and re-run reconfigure.
./configure --prefix=/usr/local/subversion1.5.4 LIBS="$LIBS -lintl"

https://svne1.access.nokiasiemensnetworks.com/isource/svnroot/IPA_TestAutomation/trunk/Tools/ipataportal/src/ipata/utils

bash-2.03# svn co https://svne1.access.nokiasiemensnetworks.com/isource/svnroot/IPA_TestAutomation/trunk/Tools/ipataportal/src/ipata/utils ipata
svn: Unrecognized URL scheme for 'https://svne1.access.nokiasiemensnetworks.com/isource/svnroot/IPA_TestAutomation/trunk/Tools/ipataportal/src/ipata/utils'
bash-2.03# svn co

#use svn --version to check supported sheme:
bash-2.03# svn --version
svn, version 1.5.4
> compiled Aug 21 2009, 13:53:25

Copyright (C) 2000-2008 CollabNet.
Subversion is open source software, see http://subversion.tigris.org/
This product includes software developed by CollabNet (http://www.Collab.Net/).

The following repository access (RA) modules are available:

`*` ra\_svn : Module for accessing a repository using the svn network protocol.
> - handles 'svn' scheme
`*` ra\_local : Module for accessing a repository on local disk.
> - handles 'file' scheme

#see a windows version subversion:
C:\Documents and Settings\dalwu>svn --version
svn, version 1.5.4 (SlikSvn:tag/1.5.4@33870) WIN32
> compiled Oct 23 2008, 23:12:07

Copyright (C) 2000-2008 CollabNet.
Subversion is open source software, see http://subversion.tigris.org/
This product includes software developed by CollabNet (http://www.Collab.Net/).

The following repository access (RA) modules are available:

`*` ra\_neon : Module for accessing a repository via WebDAV protocol using Neon.
> - handles 'http' scheme
> - handles 'https' scheme
`*` ra\_svn : Module for accessing a repository using the svn network protocol.
> - with Cyrus SASL authentication
> - handles 'svn' scheme
`*` ra\_local : Module for accessing a repository on local disk.
> - handles 'file' scheme
`*` ra\_serf : Module for accessing a repository via WebDAV protocol using serf.
> - handles 'http' scheme
> - handles 'https' scheme



build a neon to support https

> --with-neon=PREFIX      Determine neon library configuration based on
> > 'PREFIX/bin/neon-config'. Default is to search for
> > neon in a subdirectory of the top source directory
> > and then to look for neon-config in $PATH.

> --with-serf=PREFIX      Serf WebDAV client library

#install Neon on solaris:
> http://www.webdav.org/neon/neon-0.28.6.tar.gz

#gzip -d neon-0.28.6.tar.gz
#tar xvf neon-0.28.6.tar

#./configure --prefix=/usr/local/neon0.28.6 --with-ssl=openssl
checking for inflate in -lz... yes
configure: zlib support enabled, using -lz
checking whether to enable ACL support in neon... yes
checking for pkg-config... no
checking for library containing RSA\_new... not found
configure: error: could not find library containing RSA\_new
bash-2.03#

cd /opt/downloads/neon-0.28.6
./configure --prefix=/usr/local/neon0.28.3 --with-ssl=openssl --with-libs=/usr/local/ssl


#configure subversion --with-neon

./configure --prefix=/usr/local/subversion1.5.4 LIBS="$LIBS -lintl" --with-ssl --with-neon=/opt/downloads/neon-0.28.3

configure: checking neon library

An appropriate version of neon could not be found, so libsvn\_ra\_neon
will not be built.  If you want to build libsvn\_ra\_neon, please either
install neon 0.28.3 on this system

or

get neon 0.28.3 from:
> http://www.webdav.org/neon/neon-0.28.3.tar.gz
unpack the archive using tar/gunzip and rename the resulting
directory from ./neon-0.28.3/ to ./neon/

#ln -s /usr/local/neon-0.28.3/bin/neon-config /usr/local/bin/neon-config
./configure --prefix=/usr/local/subversion1.5.4 LIBS="$LIBS -lintl" --disable-neon-version-check --with-ssl