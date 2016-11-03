#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <zlib.h>

#include "gzmulti/gzmulti.h"

#include "utils.h"

int 
gzunpack (char *input_file, char *output_dir)
{

  FILE *inf = fopen (input_file, "r");
  

  if (inf == NULL)
    {
      fprintf (stderr, "ERROR: Cannot read input file %s. errno(%d)\n", input_file, errno);
      return errno;
    }

  if (mkdir (output_dir, 0777) == 0)
    {
      printf ("Creating directory %s\n", output_dir);
    }
  else // -1 will be returned.
    {
      if (errno != EEXIST) //Cannot create the directory.
        {
          fprintf (stderr, "ERROR: Cannot create directory %s. errno(%d)\n", output_dir, errno);
          return errno;
        }
    }

  /* add 3 chars in output_file for slash, off_t max digit calculations
   * adjustment, and null character. */
  char output_file[strlen (output_dir) + 1 + 1 + (int) log10 (pow (2, sizeof (off_t) * 8)) + 1];

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

  z.next_in = (Bytef *) malloc (INPUT_CHUNK * sizeof (Bytef));
  z.next_out = (Bytef *) malloc (OUTPUT_CHUNK * sizeof (Bytef));
  
  int eof = 0;
  
  while (!eof)
    {
      current_offset = ftell (inf);
      
      ret = dismissMember (&z, inf, INPUT_CHUNK, OUTPUT_CHUNK);
      
      if(ret != Z_OK)
        {
          return ret;
        }

      eof = feof (inf);
      
      next_offset = ftell (inf);
      fseek (inf, current_offset, SEEK_SET);
      sprintf (output_file, "%s/%ld.gz", output_dir, current_offset);
      
      FILE *outf = fopen (output_file, "w");
      printf ("Creating %s file\n", output_file);
      fcopy (inf, outf, next_offset - current_offset);
      fclose (outf);
    }

  free (z.next_in);
  free (z.next_out);

  (void) inflateEnd (&z);

  fclose (inf);

  return ret;
}
