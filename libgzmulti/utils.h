#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

#define IMAX (4 * 1024)
#define OMAX (8 * 1024)

/**
 * Copy count bytes from the current file position in the input to the
 * current file position in the output.
 */
size_t fcopy (FILE *ifile, FILE *ofile, size_t count);

/**
 * Return file size.
 */
size_t fsize (FILE * f);

/**
 * Update I/O max from environment variables.
 */
void getiomax (size_t *imax, size_t *omax);

#endif /* __UTILS_H__ */
