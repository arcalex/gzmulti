#include "utils.h"

void
fcopy (FILE *inf, FILE *outf, size_t size)
{
  size_t *buffer = (size_t *) malloc (size * sizeof (size_t));
  fread (buffer, 1, size, inf);
  fwrite (buffer, 1, size, outf);
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
