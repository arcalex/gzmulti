#include <gzmulti/gzmulti.h>
#include <string.h>
#include <stdlib.h>

int
main (int argc, char **argv)
{
  if (strcmp (argv[1], "unpack") == 0 && argc == 4)
    {
      return gzunpack (argv[2], argv[3]);
    }

  if (strcmp (argv[1], "delete") == 0 && argc == 5)
    {
      if (isdigit (argv[5][0]))
        {
          uInt count = atoi (argv[5]);
          return gzdelete (argv[2], argv[3], argv[4], &count);
        }
    }

  if (strcmp (argv[1], "insert") == 0 && argc == 6)
    {
      return gzinsert (argv[2], argv[3], argv[4], argv[5]);
    }

  if (strcmp (argv[1], "append") == 0 && argc == 5)
    {
      return gzappend (argv[2], argv[3], argv[4]);
    }

  if (strcmp (argv[1], "replace") == 0 && argc == 7)
    {
      if (isdigit (argv[5][0]))
        {
          uInt count = atoi (argv[5]);
          return gzreplace (argv[2], argv[3], argv[4], &count, argv[6]);
        }
    }

  return EXIT_FAILURE;
}
