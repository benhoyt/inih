AC_INIT([inih],
	m4_esyscmd([.build-aux/git-version-gen .tarball-version]),
	[https://github.com/bluesquall/inih/issues],
	[],
	[https://github.com/bluesquall/inih],
)
AC_CONFIG_AUX_DIR([.build-aux])
AC_CONFIG_MACRO_DIRS([.build-aux/m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([subdir-objects dist-xz no-dist-gzip -Wall -Wno-portability foreign])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([inih.pc:inih.pc.in])

PKG_PROG_PKG_CONFIG
PKG_INSTALLDIR

AM_PROG_AR
AC_PROG_LIBTOOL
AC_PROG_CC
AC_OUTPUT

LT_INIT([dlopen])
