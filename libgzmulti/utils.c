#include "utils.h"
#include <stdlib.h>

size_t
fcopy (FILE *ifile, FILE *ofile, size_t count)
{
  void *buffer = malloc (count);
  size_t items_read = fread (buffer, 1, count, ifile);
  size_t items_written = fwrite (buffer, 1, count, ofile);

  free (buffer);

  return items_read == items_written ? (count == items_read ? count : items_read) : -1;
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