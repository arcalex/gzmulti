#include "utils.h"
#include <stdlib.h>

size_t
fcopy (FILE *inf, FILE *outf, size_t size, int *symm)
{
  size_t window = IMAX;
  void *buffer = malloc (window);

  size_t items;
  size_t items_read = 0;
  size_t items_written = 0;

  while (!feof (inf) && size > 0)
    {
      if (size < window)
        {
          window = size;
        }
      
      size -= window;

      items = fread (buffer, 1, window, inf);
      items_read += items;
      if (ferror (inf))
        {
          free(buffer);
          break;
        }
      
      items_written += fwrite (buffer, 1, items, outf);
      if (ferror (outf))
        {
          free(buffer);
          break;
        }
    }
  
  if (symm != NULL)
    {
      *symm = (items_read == items_written);
    }
  
  free(buffer);
  return items_written;  
}

size_t
fsize (FILE *f)
{
  int pos = ftell (f);
  fseek (f, 0, SEEK_END);
  size_t size = ftell (f);
  fseek (f, pos, SEEK_SET);
  return size;
}

void
getiomax (size_t *imax, size_t *omax)
{
  char *env_imax = getenv ("GZMULTI_IMAX");
  char *env_omax = getenv ("GZMULTI_OMAX");

  *imax = env_imax ? strtoul (env_imax, NULL, 10) : IMAX;
  *omax = env_omax ? strtoul (env_omax, NULL, 10) : OMAX;

  *imax = *imax ? *imax : IMAX;
  *omax = *omax ? *omax : OMAX;
}