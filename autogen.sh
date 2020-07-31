#!/bin/bash

#     _______________________________________________________________________
#     |                                                                     |
#     |                         -*- Autogen -*-                             |
#     |_____________________________________________________________________|
#    //                                                                     \\
#   [|  GENETIC © 2014-2016 Antonio Cao (@burzumishi)                        |]
#    \\_____________________________________________________________________//

echo
echo "-- --{ Genetic }-- -* Autogen *- by (@burzumishi) --"
echo

echo "Backup configure.ac Makefile.am src/Makefile.am ...";
cp -v configure.ac configure.ac.autogen-bak
cp -v Makefile.am Makefile.am.autogen-bak
cp -v src/Makefile.am src/Makefile.am.autogen-bak

echo "Running libtoolize ..."
libtoolize

if test -f "acinclude.m4"; then rm -v acinclude.m4; fi;

if test ! -f "m4/nls.m4"; then
	if test -f "ABOUT-NLS"; then rm -v ABOUT-NLS; fi;
	if test -f "po/Makefile.in.in"; then rm -v po/Makefile.in.in; fi;

	echo "Running gettextize --copy ...";
	gettextize --copy
	
	echo "Running intltoolize --copy ...";
	intltoolize --copy --force
fi;

for file in libtool ltoptions ltsugar ltversion lt~obsolete; do
	echo "Appending m4/$file.m4 to acinclude.m4 ..."
	cat m4/$file.m4 >> acinclude.m4
done

for file in argz bison-i18n codeset expat fcntl-o gettext glib glib-gettext \
		iconv intldir intl intltool intmax inttypes_h inttypes-pri ltdl \
		lcmessage lib-ld lib-link lib-prefix libtool lock longlong ltoptions \
		ltsugar ltversion pkg po printf-posix progtest size_max stdint_h \
		threadlib uintmax_t visibility wchar_t wint_t ; do
			if test ! -f "m4/$file.m4" && test -f "/usr/share/aclocal/$file.m4"; then
				echo "Copying /usr/share/aclocal/$file.m4 -> m4/$file.m4 ..."
				cp -v /usr/share/aclocal/$file.m4 m4/$file.m4
			fi;
done;

for file in m4/Makefile.am; do
	if test ! -f "$file"; then
		echo "Creating $file ..."
		touch $file;
	fi;
done;

echo "Restore configure.ac Makefile.am src/Makefile.am ...";
cp -v configure.ac.autogen-bak configure.ac
cp -v Makefile.am.autogen-bak Makefile.am
cp -v src/Makefile.am.autogen-bak src/Makefile.am

echo "Running aclocal ...";
aclocal -I m4

echo "Running autoheader ...";
autoheader

echo "Running autoconf ...";
autoconf

for doc in INSTALL README AUTHORS NEWS ChangeLog; do
	if test ! -f "$doc"; then
		echo "Linking README.md -> $doc ..."
		cp -v README.md $doc
	fi
done

if test ! -f "COPYING"; then
	echo "Linking LICENSE -> COPYING ..."
	cp -v LICENSE COPYING
fi;

echo "Running automake ...";
automake --add-missing
automake m4/Makefile

if test ! -f "src/gettext.h"; then
	echo "Copying gettext.h -> src/ ..."
	cp -v /usr/share/gettext/gettext.h src/
fi;

test -x configure || exit

find . -type f -name "*.autogen-bak" -exec rm -v {} \;

rm -rf ltmain.sh missing install-sh config.sub config.guess compile ar-lib depcomp

cp -av /usr/share/libtool/build-aux/ltmain.sh ltmain.sh
cp -av /usr/share/automake-1.16/missing missing
cp -av /usr/share/automake-1.16/install-sh install-sh
cp -av /usr/share/automake-1.16/config.sub config.sub
cp -av /usr/share/automake-1.16/config.guess config.guess
cp -av /usr/share/automake-1.16/compile compile
cp -av /usr/share/automake-1.16/ar-lib ar-lib
cp -av /usr/share/automake-1.16/depcomp depcomp


CONFIGURE_USER_OPTIONS=$*;

echo "Running: ./configure --prefix=/usr --sysconfdir=/etc --localstatedir=/var --disable-static --enable-shared --enable-nls $CONFIGURE_USER_OPTIONS";

sleep 2;

./configure --prefix=/usr --sysconfdir=/etc --localstatedir=/var --disable-static --enable-shared --enable-nls $CONFIGURE_USER_OPTIONS;

exit

