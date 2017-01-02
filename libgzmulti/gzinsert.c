#include "utils.h"

size_t
gzinsert (char *input_file, char *output_file, char *member_file, char *off_str)
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

  size_t inserted;
  if ((inserted = fcopy (mf, outf, fsize (mf))) == -1)
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

  return inserted;
}

size_t
gzappend (char *input_file, char *output_file, char *member_file)
{
  FILE *inf = fopen (input_file, "r");
  size_t s = fsize (inf);
  fclose (inf);

  char *off_str;
  sprintf (off_str, "%ul", s);

  return gzinsert (input_file, output_file, member_file, off_str);
}