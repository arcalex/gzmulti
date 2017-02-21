#include <stdlib.h>
#include <zlib.h>

#include "utils.h"
#include "gzmulti/gzmulti.h"

int
gzreplace (char *input_file, char *output_file, char *off_str, uInt *undismissed, char *member_file)
{
  FILE *inf = fopen (input_file, "r");
  if (inf == NULL)
    {
      return Z_ERRNO;
    }

  FILE *outf = fopen (output_file, "w");
  if (outf == NULL)
    {
      return Z_ERRNO;
    }

  int symm;
  if (off_str == NULL)
    {
      fcopy (inf, outf, -1, &symm);
      if (!symm)
        {
          return Z_ERRNO;
        }
    }
  else if (!isdigit (off_str[0]))
    {
      return Z_ERRNO;
    }
  else
    {
      size_t offset = strtoul (off_str, NULL, 10);

      if (fcopy (inf, outf, offset, NULL) != offset)
        {
          return Z_ERRNO;
        }
    }

  if (undismissed != NULL)
    {
      z_stream z;
      z.zalloc = Z_NULL;
      z.zfree = Z_NULL;
      z.opaque = Z_NULL;
      z.avail_in = 0;

      int ret;
      ret = inflateInit2 (&z, 31);

      if (ret != Z_OK)
        {
          return ret;
        }

      size_t imax, omax;
      getiomax (&imax, &omax);

      /* Allocate I/O buffer in one call. */
      z.next_in = (Bytef *) malloc (imax + omax);

      if (z.next_in == NULL)
        {
          return Z_ERRNO;
        }

      /* Output buffer begins where input buffer ends. */
      z.next_out = z.next_in + imax;

      ret = dismissMembers (&z, inf, imax, omax, undismissed);

      if (ret != Z_OK)
        {
          return ret;
        }
    }

  if (member_file != NULL)
    {
      FILE *mf = fopen (member_file, "r");
      if (mf == NULL)
        {
          return Z_ERRNO;
        }

      fcopy (mf, outf, -1, &symm);
      if (!symm)
        {
          return Z_ERRNO;
        }

      fclose (mf);
    }

  if (off_str != NULL)
    {
      fcopy (inf, outf, -1, &symm);
      if (!symm)
        {
          return Z_ERRNO;
        }
    }

  fclose (inf);
  fclose (outf);

  return Z_OK;
}

int
gzdelete (char *input_file, char *output_file, char *off_str, uInt *undismissed)
{
  return gzreplace (input_file, output_file, off_str, undismissed, NULL);
}

int
gzinsert (char *input_file, char *output_file, char *off_str, char *member_file)
{
  return gzreplace (input_file, output_file, off_str, NULL, member_file);
}

int
gzappend (char *input_file, char *output_file, char *member_file)
{
  return gzreplace (input_file, output_file, NULL, NULL, member_file);
}