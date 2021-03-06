#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <zlib.h>
#include <dirent.h>
#include <sys/stat.h>

#include "gzmulti/gzmulti.h"

#include "utils.h"

int
gzunpack (char *input_file, char *output_dir)
{
  FILE *inf = fopen (input_file, "r");

  if (inf == NULL)
    {
      return Z_ERRNO;
    }

  /*
   * Maximum possible length of output file path:
   *   output_dir + "/" + off_t + ".gz" + '\0'
   */
  char output_file[strlen (output_dir) +
          1 +
          (int) ceil (log10 (pow (2, sizeof (off_t) * 8))) +
          3 +
          1];

  off_t current_offset = 0;
  off_t next_offset = 0;

  z_stream z;
  z.zalloc = Z_NULL;
  z.zfree = Z_NULL;
  z.opaque = Z_NULL;
  z.avail_in = 0;

  int ret = inflateInit2 (&z, 31);

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

  int eof = 0;

  while (!eof)
    {
      current_offset = ftell (inf);

      ret = dismissMember (&z, inf, imax, omax);

      if (ret != Z_OK)
        {
          return ret;
        }

      eof = feof (inf);

      next_offset = ftell (inf);
      fseek (inf, current_offset, SEEK_SET);
      sprintf (output_file, "%s/%ld.gz", output_dir, current_offset);

      FILE *outf = fopen (output_file, "w");

      if (outf == NULL)
        {
          return Z_ERRNO;
        }

      if (fcopy (inf, outf, next_offset - current_offset, NULL) != next_offset - current_offset)
        {
          return Z_ERRNO;
        }

      fclose (outf);
    }

  free (z.next_in);

  (void) inflateEnd (&z);

  fclose (inf);

  return ret;
}

int
gzpack (char *input_dir, char *output_file)
{
  struct dirent **namelist;
  int dirent_count = scandir (input_dir, &namelist, 0, versionsort);

  if (dirent_count < 0)
    {
      perror (input_dir);
      return Z_ERRNO;
    }

  char pathname[PATH_MAX];
  FILE *inf;
  FILE *outf = fopen (output_file, "a");
  struct stat s;
  int symm;
  int i;

  for (i = 0; i < dirent_count; i++)
    {
      sprintf (pathname, "%s/%s", input_dir, namelist[i]->d_name);

      /* Skip current directory, parent directory, and any subdirectories. */

      if (!stat (pathname, &s))
        {
          perror (pathname);
          return Z_ERRNO;
        }

      if (!strcmp (namelist[i]->d_name, ".") ||
          !strcmp (namelist[i]->d_name, "..") ||
          S_ISDIR (s.st_mode))
        {
          continue;
        }

      inf = fopen (pathname, "r");
      fcopy (inf, outf, -1, &symm);

      if (!symm)
        {
          return Z_ERRNO;
        }

      fclose (inf);
      free (namelist[i]);
    }

  fclose (outf);
  free (namelist);

  return Z_OK;
}
