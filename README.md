GZMulti for Manipulating Multi-Member GZIP
==========================================

gzmulti is a tool for working with multi-member GZIP files.  It is
intended to supplement the standard gzip(1) and gunzip(1) tools.
Applications where multi-member GZIP is used include .warc.gz files used
in web archiving and also initrd files used in the Linux boot process.

gzmulti aims to be fast and lightweight.  It is written in C and depends
on zlib.

In addition to the tool, a library (libgzmulti) is provided.  Currently,
the library implements a single function: inflateMember().

inflateMember() iterates through a z_stream until one member is inflated
then returns.  After each write to the output buffer, a callback
function is invoked, allowing the caller the opportunity to process the
decompressed data.  The usual z_stream initialization is required before
calling inflateMember().

This is a usage example for inflateMember():

```
#include <zlib.h>
#include "gzmulti.h"

int
main (int argc, char **argv)
{
  FILE *f;

  z_stream z;
  z.zalloc = Z_NULL;
  z.zfree = Z_NULL;
  z.opaque = Z_NULL;

  z.avail_in = 0;
  z.next_in = Z_NULL;

  int ret;

  /*
   * "windowBits can also be greater than 15 for optional gzip decoding.
   * Add 32 to windowBits to enable zlib and gzip decoding with
   * automatic header detection, or add 16 to decode only the gzip
   * format (the zlib format will return a Z_DATA_ERROR)."
   */

  ret = inflateInit2 (z, 31);

  if (ret != Z_OK)
    {
      return 1;
    }

  z.next_out =
    (Bytef *) calloc (8 * 1024, sizeof (Bytef));

  z.next_in =
    (Bytef *) calloc (4 * 1024, sizeof (Bytef));
  f = fopen (argv[1], "r");


}

void procMember (z_streamp z, int chunk, void *userPtr)
{
}

```

gzmulti was developed by the Bibliotheca Alexandrina.
