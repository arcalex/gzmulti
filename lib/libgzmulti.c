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

#define MAX_OUT 16 * 1024

/*
 * Inflate one member then stop. Currently, this attempts to load the
 * entire member into the specified memory buffer (next_out). If the
 * length of the memory buffer (avail_out) is too small to fit the
 * uncompressed data, the function returns an error. Obviously,
 * attempting to load the entire member into memory is wrong, because
 * should the member be too big for memory, things will go bad.
 */
int
inflateMember (FILE *f, z_stream *z, Bytef *next_out, uLong avail_out)
{
  /* Block of bytes read from file to be inflated. */
  Bytef in[MAX_OUT];

  /* Status returned by zlib functions. */
  int ret;

  /* Initialize z variables. */

  z->zalloc = Z_NULL;
  z->zfree = Z_NULL;
  z->opaque = Z_NULL;

  z->avail_in = 0;
  z->next_in = Z_NULL;

  /* 47 = 15 + 32, length of the GZIP header. */
  ret = inflateInit2 (z, 47);

  if (ret != Z_OK)
    {
      fprintf (stderr, "inflateInit2 failed, Z_ERRNO = %d\n", Z_ERRNO);
      return ret;
    }

  /* Read chunks from file and inflate until end of stream or end of
   * file. */
  do
    {
      /* avail_in = number of bytes available at next_in. */
      z->avail_in = fread (in, 1, MAX_OUT, f);

      if (ferror (f))
        {
          (void) inflateEnd (z);
          return Z_STREAM_ERROR;
        }

      if (z->avail_in == 0)
        {
          break;
        }

      z->next_in = in;
      z->avail_out = avail_out;
      z->next_out = next_out;


      ret = inflate (z, Z_NO_FLUSH);

      switch (ret)
        {
        case Z_NEED_DICT:
          ret = Z_DATA_ERROR;
          return ret;
        case Z_DATA_ERROR:
          return ret;
        case Z_MEM_ERROR:
          (void) inflateEnd (z);
          return ret;
        }

      fprintf (stderr, "ftell %l  avail_in %d  avail_out %d  ret %d  msg: \"%s\"\n", ftell (f), z->avail_in, z->avail_out, ret, z->msg);
    }
  while (z->avail_out != 0 && ret != Z_STREAM_END);

  (void) inflateEnd (z);
  fseek (f, -1 * (int) z->avail_in, SEEK_CUR);

  if (z->avail_out == 0 && ret != Z_STREAM_END)
    {
      /* Member too big for output buffer. */
    }

  return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}
