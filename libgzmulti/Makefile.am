gzmultiincludedir = -I$(top_builddir)/include -I$(top_srcdir)/include

lib_LTLIBRARIES = libgzmulti.la

libgzmulti_la_SOURCES = libgzmulti.c utils.h utils.c gzunpack.c gzreplace.c

libgzmulti_la_LIBADD = -lz

AM_CPPFLAGS = $(gzmultiincludedir)
