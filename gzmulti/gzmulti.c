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
  else
    {
      return EXIT_FAILURE;
    }
}
