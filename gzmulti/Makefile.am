gzmultiincludedir = -I$(top_builddir)/include -I$(top_srcdir)/include

bin_PROGRAMS = gzmulti

gzmulti_SOURCES = gzmulti.c

gzmulti_LDADD = -lz $(top_builddir)/libgzmulti/libgzmulti.la

AM_CPPFLAGS = $(gzmultiincludedir)
