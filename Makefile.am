ACLOCAL_AMFLAGS = -I .build-aux/m4

lib_LTLIBRARIES = libinih.la

pkgconfig_DATA = inih.pc

dist_doc_DATA = README.md LICENSE.txt

# dist_noinst_DATA = meson.build meson_options.txt

EXTRA_DIST = .build-aux/git-version-gen

pkginclude_HEADERS = ini.h
libinih_la_SOURCES = ini.c

libinih_la_CFLAGS = $(AM_CFLAGS)
libinih_la_LDFLAGS = -module -shared -export-dynamic -version-info 0:1
