#include "gzmulti/gzmulti.h"

/**
 * This library is an offshoot of the initial gzmulti tool
 * implementation. The idea is to build an easy-to-use wrapper API for
 * working with multi-member GZIP files.
 *
 * In addition to using the initial gzmulti source code as reference,
 * consult the zlib manual:
 *
 * http://www.zlib.net/manual.html
 */

int
inflateMember (z_stream *z, FILE *f, size_t max_in, size_t max_out, void (*procMember) (z_stream *, int, void *), void *userPtr)
{
  /* Status returned by zlib functions. */
  int ret = Z_STREAM_END;

  Bytef *next_in = z->next_in, *next_out = z->next_out;

  int chunk = CHUNK_FIRST;

  /* Read chunks from file and inflate until end of stream or end of
   * file. */
  do
    {
      z->next_in = next_in;

      /* avail_in = number of bytes available at next_in. */
      z->avail_in = fread (z->next_in, 1, max_in, f);

      if (ferror (f))
        {
          (void) inflateEnd (z);
          return Z_ERRNO;
        }

      if (z->avail_in == 0)
        {
          break;
        }

      do
        {
          z->avail_out = max_out;

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

          z->next_out = next_out;

          if (procMember == NULL)
            {
              continue;
            }

          if (z->avail_out != 0 && ret == Z_STREAM_END)
            {
              if (chunk == CHUNK_FIRST)
                {
                  chunk = CHUNK_FIRST_LAST;
                }
              else if (chunk == CHUNK_MIDDLE)
                {
                  chunk = CHUNK_LAST;
                }
            }

          procMember (z, chunk, userPtr);

          chunk = CHUNK_MIDDLE;
        }
      while (z->avail_out == 0 && ret != Z_STREAM_END);
    }
  while (ret != Z_STREAM_END);

  if (z->avail_in)
    {
      fseek (f, -1 * (int) z->avail_in, SEEK_CUR);
    }

  z->next_in = next_in;
  z->next_out = next_out;

  ret = inflateReset2 (z, 31);

  return ret;
}

int
dismissMembers (z_stream *z, FILE *f, size_t max_in, size_t max_out, uInt *undismissed)
{
  int dismiss_failed = Z_OK;

  while ((*undismissed) && !dismiss_failed && !feof (f))
    {
      (*undismissed)--;
      dismiss_failed = inflateMember (z, f, max_in, max_out, NULL, NULL);
    }

  return dismiss_failed;
}

int
dismissMember (z_stream *z, FILE *f, size_t max_in, size_t max_out)
{
  uInt undismissed = 1;
  return dismissMembers (z, f, max_in, max_out, &undismissed);
}
