#     |                                                                     |
#     |                         -*- configure.ac -*-                        |
#     |                                                                     |
#     |   Process this file with autoconf to produce a configure script     |
#     |_____________________________________________________________________|
#    //                                                                     \\
#   [|  GENETIC © 2014-2019 Antonio Cao (@burzumishi)                        |]
#    \\_____________________________________________________________________//

AC_PREREQ([2.69])

AC_INIT([genetic],[20.03.08],[<@burzumishi>],[genetic])

AC_COPYRIGHT(["2014-2020 (c) Antonio Cao (@burzumishi)"])

AC_REVISION([$Revision: 1.20 $])

AC_SUBST([DESCRIPTION], ["Genetic Package Manager"])

AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_HEADERS([src/genetic.h])


AM_INIT_AUTOMAKE([1.9 no-dist-gzip dist-bzip2 dist-xz tar-ustar -Wno-portability])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
AC_CONFIG_MACRO_DIR([m4])

AM_MAINTAINER_MODE([disable])

AH_TOP([
#ifndef GENETIC_CONFIG_H__
#define GENETIC_CONFIG_H__
])

AH_BOTTOM([
#endif /* GENETIC_CONFIG_H__ */
])

AM_SANITY_CHECK

AC_CANONICAL_HOST
AC_CANONICAL_BUILD

AC_DEFINE_UNQUOTED([PREFIX], ["/usr/"], [Define to the root directory.])
AC_DEFINE_UNQUOTED([SBINDIR], ["${prefix}/sbin/"], [Define Executable Directory.])

AC_LANG([C])
AC_LANG_PUSH([C])

AC_PROG_CC([cc])
AC_PROG_CXX([g++])

AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AM_PROG_CC_C_O
AC_PROG_LEX
AC_PROG_AWK
AC_PROG_GREP
AC_PROG_SED
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
AM_PROG_AS

AC_FUNC_MALLOC

AC_SUBST(ACLOCAL_AMFLAGS, "\${ACLOCAL_FLAGS}")

LT_PREREQ([2.4])  # require libtool
LT_INIT()
LT_LANG([C])

AC_SUBST(LIBTOOL_DEPS)

# Checks for libraries.

AM_WITH_DMALLOC

AC_PATH_PROG([GNUMAKE], [make])
AM_CONDITIONAL([HAVE_GNUMAKE], [test -n "${GNUMAKE}"])

AC_PATH_PROG([FILE], [file])
AM_CONDITIONAL([FILE], [test -n "${FILE}"])

AC_PATH_PROG([BASH], [bash])
AM_CONDITIONAL([HAVE_BASH], [test -n "${BASH}"])

AC_PATH_PROG([ENV], [env])
AM_CONDITIONAL([HAVE_ENV], [test -n "${ENV}"])

AC_PATH_PROG([TAIL], [tail])
AM_CONDITIONAL([HAVE_TAIL], [test -n "${TAIL}"])

AC_PATH_PROG([HEAD], [head])
AM_CONDITIONAL([HAVE_HEAD], [test -n "${HEAD}"])

AC_PATH_PROG([LESS], [less])
AM_CONDITIONAL([HAVE_LESS], [test -n "${LESS}"])

AC_PATH_PROG([MORE], [more])
AM_CONDITIONAL([HAVE_MORE], [test -n "${MORE}"])

AC_PATH_PROG([FIND], [find])
AM_CONDITIONAL([HAVE_FIND], [test -n "${FIND}"])

AC_PATH_PROG([SED], [sed])
AM_CONDITIONAL([HAVE_SED], [test -n "${SED}"])

AC_PATH_PROG([TAR], [tar])
AM_CONDITIONAL([HAVE_TAR], [test -n "${TAR}"])

AC_PATH_PROG([GZIP], [gzip])
AM_CONDITIONAL([HAVE_GZIP], [test -n "${GZIP}"])

AC_PATH_PROG([BZIP2], [bzip2])
AM_CONDITIONAL([HAVE_BZIP2], [test -n "${BZIP2}"])

AC_PATH_PROG([XZ], [xz])
AM_CONDITIONAL([HAVE_XZ], [test -n "${XZ}"])

AC_PATH_PROG([PERL], [perl])
AM_CONDITIONAL([HAVE_PERL], [test -n "${PERL}"])

AC_PATH_PROG([PATCH], [patch])
AM_CONDITIONAL([HAVE_PATCH], [test -n "${PATCH}"])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
AC_TYPE_INT64_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_STRUCT_TIMEZONE

AC_C_CONST
AC_C_INLINE

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MKTIME
AC_FUNC_REALLOC

AC_CHECK_FUNCS([bzero gethostname re_comp setlocale textdomain bindtextdomain alarm dup2 gethostbyaddr gethostbyname gettimeofday inet_ntoa isascii memmove memset pow putenv select socket sqrt strcasecmp strchr strerror strpbrk strrchr strstr tzset strtol getcwd memchr mkdir rmdir strcspn strdup strspn])

dnl Language Support
IT_PROG_INTLTOOL([0.50.1])

dnl Check for gettext
GETTEXT_PACKAGE=genetic2
AM_GNU_GETTEXT([external])
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [The gettext translation domain])
AC_SUBST(GETTEXT_PACKAGE)

dnl Dependency Checks
PKG_PROG_PKG_CONFIG()

# Check for doxygen (optional) #
AC_CHECK_PROGS([DOXYGEN], [doxygen])
AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "${DOXYGEN}"])

# Check for bash (required) #
AC_PATH_PROG([BASH], [bash])
AM_CONDITIONAL([HAVE_BASH], [test -n "${BASH}"])

AC_CHECK_PROG(BASH, [bash], [yes], [no])
if test "$BASH" = "no"; then
  AC_MSG_ERROR([Unable to find the bash command.])
fi

# Check for env (required) #
AC_PATH_PROG([ENV], [env])
AM_CONDITIONAL([HAVE_ENV], [test -n "${ENV}"])

AC_CHECK_PROG(ENV, [env], [yes], [no])
if test "$ENV" = "no"; then
  AC_MSG_ERROR([Unable to find the env command.])
fi

# Check for make (required) #
AC_PATH_PROG([GNUMAKE], [make])
AM_CONDITIONAL([HAVE_GNUMAKE], [test -n "${GNUMAKE}"])

AC_CHECK_PROG(GNUMAKE, [make], [yes], [no])
if test "$GNUMAKE" = "no"; then
  AC_MSG_ERROR([Unable to find the make command.])
fi

# Check for perl (required) #
AC_PATH_PROG([PERL], [perl])
AM_CONDITIONAL([HAVE_PERL], [test -n "${PERL}"])

AC_CHECK_PROG(PERL, [perl], [yes], [no])
if test "$PERL" = "no"; then
  AC_MSG_ERROR([Unable to find the perl command.])
fi

## CPU architecture specific assembly

build_cpu_mmx="no"
build_cpu_sse3="no"
build_cpu_altivec="no"
build_cpu_neon="no"

want_neon="yes"
AC_ARG_ENABLE([neon],
   [AS_HELP_STRING([--disable-neon],[disable neon support @<:@default=enable@:>@])],
   [
    if test "x${enableval}" = "xyes"; then
       want_neon="yes"
    else
       want_neon="no"
    fi
   ])

SSE3_CFLAGS=""
ALTIVEC_CFLAGS=""
NEON_CFLAGS=""

case $host_cpu in
  i*86|x86_64|amd64)
    AC_DEFINE([BUILD_MMX], [1], [Build MMX Code])
    build_cpu_mmx="yes"
    AC_CHECK_HEADER([immintrin.h],
       [
        AC_DEFINE(BUILD_SSE3, 1, [Build SSE3 Code])
        build_cpu_sse3="yes"
       ],
       [build_cpu_sse3="no"])
    AC_MSG_CHECKING([whether to build SSE3 code])
    AC_MSG_RESULT([${build_cpu_sse3}])

    if test "x$build_cpu_sse3" = "xyes" ; then
       SSE3_CFLAGS="-msse3"
    fi
    ;;
  *power* | *ppc*)
    build_cpu_altivec="yes"
    AC_CHECK_HEADER([altivec.h],
       [
        AC_DEFINE([BUILD_ALTIVEC], [1], [Build Altivec Code])
        AC_DEFINE([HAVE_ALTIVEC_H], [1], [Have altivec.h header file])
        build_cpu_altivec="yes"
       ],
       [
        save_CFLAGS=$CFLAGS
        save_CPPFLAGS=$CPPFLAGS
        CFLAGS=$CFLAGS" -maltivec"
        CPPFLAGS=$CPPFLAGS" -maltivec"
        unset ac_cv_header_altivec_h
        AC_CHECK_HEADER([altivec.h],
          [
            AC_DEFINE([BUILD_ALTIVEC], [1], [Build Altivec Code])
            AC_DEFINE([HAVE_ALTIVEC_H], [1], [Have altivec.h header file])
            build_cpu_altivec="yes"
          ],
          [build_cpu_altivec="no"]
        )
        CFLAGS=$save_CFLAGS
        CPPFLAGS=$save_CPPFLAGS
       ]
    )
    if test "x${build_cpu_altivec}" = "xyes"; then
       AC_MSG_CHECKING([whether to use altivec compiler flag])
       if test "x$GCC" = "xyes"; then
          if echo "int main(){return 0;}" | ${CPP} -faltivec - > /dev/null 2>&1; then
             altivec_cflags="-faltivec"
             AC_DEFINE([BUILD_ALTIVEC], [1], [Build Altivec Code])
          elif echo "int main(){return 0;}" | ${CPP} -maltivec - > /dev/null 2>&1; then
             altivec_cflags="-maltivec"
             AC_DEFINE([BUILD_ALTIVEC], [1], [Build Altivec Code])
          fi
       fi
       AC_MSG_RESULT([${altivec_cflags}])
       CFLAGS="$CFLAGS ${altivec_cflags}"
       ALTIVEC_CFLAGS="-maltivec"
    fi
    ;;
  arm*)
    if test "x${want_neon}" = "xyes"; then
       build_cpu_neon="yes"
       AC_MSG_CHECKING([whether to use NEON instructions])
       CFLAGS_save="${CFLAGS}"
       CFLAGS="${CFLAGS} -mfpu=neon -ftree-vectorize"
       AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <arm_neon.h>]], [[asm volatile ("vqadd.u8 d0, d1, d0\n")]])],[
           AC_MSG_RESULT([yes])
           AC_DEFINE([BUILD_NEON], [1], [Build NEON Code])
           build_cpu_neon="yes"
           NEON_CFLAGS="-mfpu=neon"
       	 ],[
	   AC_MSG_RESULT([no])
           build_cpu_neon="no"
       	 ])
       CFLAGS="${CFLAGS_save}"
    fi
    ;;
esac

AC_SUBST([ALTIVEC_CFLAGS])
AC_SUBST([SSE3_CFLAGS])
AC_SUBST([NEON_CFLAGS])

# sockets

case "$host_os" in
	*mingw*)
	    AC_MSG_NOTICE([OS MinGW detected])
      AC_DEFINE_UNQUOTED(MINGW,1,[This is a MinGW system])
      AC_DEFINE_UNQUOTED(WIN32,1,[This is a Windows system])
      AC_DEFINE_UNQUOTED(WINDOWS,1,[This is a Windows system])
      LIBS="$LIBS -lws2_32"
      AC_CHECK_HEADERS([winsock2.h ws2tcpip.h],, AC_MSG_ERROR([[Winsock2 headers are required for W32]]))
      AC_CACHE_CHECK([for MS lib utility], [ac_cv_use_ms_lib_tool],
       [[mslibcheck=`lib 2>&1`
        if [[ $mslibcheck = "Microsoft (R) Library Manager"* ]]; then
          ac_cv_use_ms_lib_tool="yes"
        else
          ac_cv_use_ms_lib_tool="no"
        fi
         ]])
      if test "x$ac_cv_use_ms_lib_tool" = "xyes"; then
        AC_SUBST([MS_LIB_TOOL], [[lib]])
      fi
      AC_SUBST([lt_cv_objdir])
      os_is_windows="yes"
      os_is_native_w32="yes"
      have_socket="no"
	    enable_crypt="no"
      ;;
	freebsd*)
	    AC_MSG_NOTICE([OS FreeBSD detected])
      AC_DEFINE_UNQUOTED(SOMEBSD,1,[This is a BSD system])
      AC_DEFINE_UNQUOTED(FREEBSD,1,[This is a FreeBSD system])
      ;;
	openbsd*)
	    AC_MSG_NOTICE([OS OpenBSD detected])
      AC_DEFINE_UNQUOTED(SOMEBSD,1,[This is a BSD system])
      AC_DEFINE_UNQUOTED(OPENBSD,1,[This is an OpenBSD system])
      ;;
	netbsd*)
	    AC_MSG_NOTICE([OS NetBSD detected])
      AC_DEFINE_UNQUOTED(SOMEBSD,1,[This is a BSD system])
      AC_DEFINE_UNQUOTED(NETBSD,1,[This is a NetBSD system])
      ;;

	*solaris*)
	    AC_MSG_NOTICE([OS Solaris detected])
      AC_DEFINE_UNQUOTED(SOLARIS,1,[This is a Solaris system])
      AC_DEFINE_UNQUOTED(_REENTRANT,1,[Need with solaris or errno doesnt work])
      AC_SEARCH_LIBS(gethostbyname, nsl)
      AC_SEARCH_LIBS(socket, socket)
      AC_CHECK_LIB([socket], [connect],
         [
          have_socket="yes"
          requirement_elm_libs="-lsocket ${requirement_elm_libs}"
         ],
         [have_socket="no"])
      CFLAGS="-Dsun -DSYSV $CFLAGS"
      LDFLAGS="-lnsl -lsocket $LDFLAGS"
      ;;
  *darwin* | *rhapsody* | *macosx*)
	    AC_MSG_NOTICE([OS MacOSX detected])
      AC_DEFINE_UNQUOTED(OSX,1,[This is an OS X system])
      CFLAGS="-no-cpp-precomp -fno-common $CFLAGS"
      have_socket="yes"
      ;;
	*arm-linux*)
	    AC_MSG_NOTICE([OS ARM GNU/Linux detected])
      AC_DEFINE_UNQUOTED(LINUX,1,[This is a Linux kernel])
      AC_DEFINE_UNQUOTED(HAVE_LISTEN_SHUTDOWN,1,[can use shutdown on listen sockets])
      CFLAGS="-fPIC -pipe $CFLAGS"
      ;;
	*linux*)
	    AC_MSG_NOTICE([OS GNU/Linux detected])
      AC_DEFINE_UNQUOTED(LINUX,1,[This is a Linux kernel])
      AC_DEFINE_UNQUOTED(HAVE_LISTEN_SHUTDOWN,1,[can use shutdown on listen sockets])
      ;;
	*cygwin*)
	    AC_MSG_NOTICE([OS Cygwin detected])
      AC_DEFINE_UNQUOTED(__CYGWIN__,1,[This is a Cygwin system])
      os_is_windows="yes"
      have_socket="yes"
	    enable_crypt="no"
      enable_regex="no"
      ;;
  *openedition*)
	    AC_MSG_NOTICE([OS Open Edition detected])
      AC_DEFINE_UNQUOTED(OS390,1,[This is a OS/390 system])
      ;;
  *)
      AC_MSG_WARN([Unrecognised OS $host_os])
      AC_DEFINE_UNQUOTED(OTHEROS,1,[Some strange OS])
      have_socket="yes"
      ;;
esac

AM_CONDITIONAL([BUILD_RUN], [test "x$have_socket" = "xyes"])

### GENETIC BASIC FEATURES ###

# Profiling Support #
AC_ARG_ENABLE(profiling,
              AS_HELP_STRING([--enable-profiling],[enable profiling support @<:@default=no@:>@]),
              [], [enable_profiling=no])
if test "$enable_profiling" = "yes"; then
		CFLAGS="$CFLAGS -pg -g3"
else
		CFLAGS="$CFLAGS -fomit-frame-pointer"
fi

### GENETIC BUILD FLAGS ###

# Performance CFLAGS #
AC_ARG_ENABLE(performance,
              AS_HELP_STRING([--enable-performance],[enable Performance Flags @<:@default=yes@:>@]),
              [], [enable_performance=yes])
if test "$enable_performance" = "yes"; then
		LDFLAGS="$LDFLAGS -finline-functions -funroll-loops -fdefer-pop -fstrength-reduce"
fi

# Developer CFLAGS #
AC_ARG_ENABLE(devel,
              AS_HELP_STRING([--enable-devel],[enable Developer Build Flags @<:@default=no@:>@]),
              [], [enable_devel=no])
if test "$enable_devel" = "yes"; then
		CFLAGS="$CFLAGS -Wall -Wshadow -Wformat-security -Wpointer-arith -Wcast-align -Wredundant-decls -Wuninitialized"
else
		CFLAGS="$CFLAGS -Wno-all -Wno-cast-align -Wno-unused-result -Wno-write-strings -Wno-unused-but-set-variable -Wno-pointer-to-int-cast -Wno-address -Wno-int-to-pointer-cast -Wno-uninitialized"
fi

### GENETIC 2.0 EXTRA FEATURES ###

# GENETIC 2.0 Suport (-DGENETIC -DGENETIC2) #
AC_ARG_ENABLE(genetic2,
              AS_HELP_STRING([--enable-genetic2],[enable GENETIC 2.0 support @<:@default=yes@:>@]),
              [], [enable_genetic2=yes])
if test "$enable_genetic2" = "yes"; then
		AC_DEFINE([GENETIC2], [], [GENETIC Support])
fi
AM_CONDITIONAL([GENETIC2], [test "$enable_genetic2" = "yes"])

# Create build files #

AC_CONFIG_FILES([Makefile
		 po/Makefile.in
		 src/Makefile
		 etc/Makefile
		 etc/conf.d/Makefile])
LT_OUTPUT()
AC_OUTPUT

# Show configure build flags & options #

AC_MSG_RESULT
AC_MSG_RESULT([Configured --{ GENETIC }-- build flags:

 * Profiling flags:     <$enable_profiling>
 * Developer flags:     <$enable_devel>
 * Performance flags:   <$enable_performance>
 * Genetic ${PACKAGE_VERSION}:    <$enable_genetic2>
])

AC_MSG_RESULT([Well done! \"configure\" complete, now type \"make\"!])
AC_MSG_RESULT
