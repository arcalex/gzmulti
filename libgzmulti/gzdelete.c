#include <stdlib.h>
#include <zlib.h>

#include "gzmulti/gzmulti.h"

size_t
gzdelete (char *input_file, char *output_file, char *off_str)
{
  if (!isdigit (off_str[0]))
    {
      return 0;
    }

  size_t offset = strtoul (off_str, NULL, 10);

  FILE *inf = fopen (input_file, "r");
  if (inf == NULL)
    {
      return 0;
    }

  FILE *outf = fopen (output_file, "w");
  if (outf == NULL)
    {
      return 0;
    }

  if (fcopy (inf, outf, offset) == -1)
    {
      return 0;
    }

  z_stream z;
  z.zalloc = Z_NULL;
  z.zfree = Z_NULL;
  z.opaque = Z_NULL;
  z.avail_in = 0;

  int ret;
  ret = inflateInit2 (&z, 31);

  if (ret != Z_OK)
    {
      return 0;
    }

  size_t imax, omax;
  getiomax (&imax, &omax);

  /* Allocate I/O buffer in one call. */
  z.next_in = (Bytef *) malloc (imax + omax);

  if (z.next_in == NULL)
    {
      return 0;
    }

  /* Output buffer begins where input buffer ends. */
  z.next_out = z.next_in + imax;

  ret = dismissMember (&z, inf, imax, omax);

  if (ret != Z_OK)
    {
      return 0;
    }

  size_t dismissed = ftell (inf) - offset;
  if (fcopy (inf, outf, fsize (inf) - ftell (inf)) == -1)
    {
      return 0;
    }

  fclose (inf);
  fclose (outf);

  return dismissed;
}

size_t
gzreplace (char *input_file, char *output_file, char *member_file, char *off_str, uInt *undismissed)
{
  if (!isdigit (off_str[0]))
    {
      return 0;
    }

  size_t offset = strtoul (off_str, NULL, 10);

  FILE *inf = fopen (input_file, "r");
  if (inf == NULL)
    {
      return 0;
    }

  FILE *mf = fopen (member_file, "r");
  if (mf == NULL)
    {
      return 0;
    }

  FILE *outf = fopen (output_file, "w");
  if (outf == NULL)
    {
      return 0;
    }

  if (fcopy (inf, outf, offset) == -1)
    {
      return 0;
    }

  z_stream z;
  z.zalloc = Z_NULL;
  z.zfree = Z_NULL;
  z.opaque = Z_NULL;
  z.avail_in = 0;

  int ret;
  ret = inflateInit2 (&z, 31);

  if (ret != Z_OK)
    {
      return 0;
    }

  size_t imax, omax;
  getiomax (&imax, &omax);

  /* Allocate I/O buffer in one call. */
  z.next_in = (Bytef *) malloc (imax + omax);

  if (z.next_in == NULL)
    {
      return 0;
    }

  /* Output buffer begins where input buffer ends. */
  z.next_out = z.next_in + imax;

  ret = dismissMembers (&z, inf, imax, omax, undismissed);

  if (ret != Z_OK)
    {
      return 0;
    }

  size_t dismissed = ftell (inf) - offset;

  if ((fcopy (mf, outf, fsize (mf))) == -1)
    {
      return 0;
    }

  if (fcopy (inf, outf, fsize (inf) - ftell (inf)) == -1)
    {
      return 0;
    }

  fclose (inf);
  fclose (mf);
  fclose (outf);

  return dismissed;
}