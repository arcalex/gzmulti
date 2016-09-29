#include <gzmulti/gzmulti.h>

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
 * Inflate one member then stop.  As the function iterates through the
 * stream, up to max_in bytes are read, inflated, and up to max_out
 * bytes are written.  The callback function procMember is invoked after
 * each inflate, giving the caller the opportunity to, for instance,
 * write the inflated data to a file.
 */
int
inflateMember (z_stream *z, FILE *f, unsigned int max_in, unsigned int max_out, void (*procMember) (z_stream *, int, void *), void *userPtr)
{
  /* Status returned by zlib functions. */
  int ret;

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
          return Z_STREAM_ERROR;
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

          if (procMember != NULL)
            {
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
        }
      while (z->avail_out == 0 && ret != Z_STREAM_END);
    }
  while (ret != Z_STREAM_END);

  (void) inflateEnd (z);
  fseek (f, -1 * (int) z->avail_in, SEEK_CUR);

  z->next_in = next_in;
  z->next_out = next_out;

  return ret;
}
