GZMulti for Manipulating Multi-Member GZIP
==========================================

gzmulti is a tool for working with multi-member GZIP files.  It is
intended to supplement the standard gzip(1) and gunzip(1) tools.
Applications where multi-member GZIP is used include .warc.gz files used
in web archiving and also initrd files used in the Linux boot process.

gzmulti aims to be fast and lightweight.  It is written in C and depends
on [zlib](http://www.zlib.net/).

In addition to the tool, a library
[libgzmulti](include/gzmulti/gzmulti.h) is provided.  Currently,
the library implements the following functions:

* [gzunpack](libgzmulti/gzunpack.c)
* [gzpack](libgzmulti/gzunpack.c)
* [gzreplace](libgzmulti/gzreplace.c)
* [gzdelete](libgzmulti/gzreplace.c)
* [gzinsert](libgzmulti/gzreplace.c)
* [gzappend](libgzmulti/gzreplace.c)

These functions depend on the implementation of
[inflateMember](libgzmulti/libgzmulti.c) and its wrapper functions,
[dismissMembers](libgzmulti/libgzmulti.c) and
[dismissMember](libgzmulti/libgzmulti.c).

[inflateMember](libgzmulti/libgzmulti.c) iterates through a z_stream
until one member is inflated then returns.  After each write to the
output buffer, a callback function is invoked, allowing the caller the
opportunity to process the decompressed data.  The usual z_stream
initialization is required before calling
[inflateMember](libgzmulti/libgzmulti.c).

You can find the usage example in [gzmulti.c](gzmulti/gzmulti.c).

gzmulti was developed by [the Bibliotheca
Alexandrina](http://www.bibalex.org/).
