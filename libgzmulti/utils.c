#include "utils.h"

void
fcopy (FILE *ifile, FILE *ofile, size_t count)
{
  void *buffer = malloc (count);
  fread (buffer, 1, count, ifile);
  fwrite (buffer, 1, count, ofile);
  free (buffer);
}

size_t
fsize (FILE * f)
{
  fseek (f, 0, SEEK_END);
  size_t size = ftell (f);
  rewind (f);
  return size;
}
