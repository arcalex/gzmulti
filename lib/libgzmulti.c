#include "gzmulti.h"

/*
 * This library is an offshoot of the initial gzmulti tool
 * implementation. The idea is to build an easy-to-use wrapper API for
 * working with multi-member GZIP files.
 *
 * In addition to using the initial gzmulti source code as reference,
 * consult the zlib manual:
 *
 * http://www.zlib.net/manual.html
 */

/*
 * Should we just get rid of this function?
 */
FILE *
gzmopen (const char *path, const char *mode)
{
  return fopen (path, mode);
}

/*
 * Should we just get rid of this function?
 */
int
gzmclose (FILE *fp)
{
  return fclose (fp);
}

/*
 * Inflate one member then stop. Currently, this attempts to load the
 * entire member into the specified memory buffer (obuf). If the length
 * of the memory buffer (obuf_len) is too small to fit the uncompressed
 * data, the function returns an error. Obviously, attempting to load
 * the entire member into memory is wrong, because should the member be
 * too big for memory, things will go bad.
 */
int
gzminflate (FILE *fp, Bytef *obuf, uLong obuf_len)
{
  /* zlib struct. */
  z_stream z;

  /* Holds block of bytes read from file to be decompressed. */
  Bytef ibuf[GZM_BUF_SIZE];

  /* Status returned by zlib functions. */
  int err;

  /* Internal state alloc/free. */
  z.zalloc = Z_NULL;
  z.zfree = Z_NULL;
  z.opaque = Z_NULL;

  z.avail_in = 0;
  z.next_in = Z_NULL;

  /* 47 = 15 + 32, length of the GZIP header. */
  err = inflateInit2 (&z, 47);

  /* TODO: This function is not finished yet!! */

  
}
