#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

#define MAX_IN  (4 * 1024)
#define MAX_OUT (8 * 1024)

/* 
 * Copy count bytes from the current file position in the input to the
 * current file position in the output.
 */
void fcopy (FILE *ifile, FILE *ofile, size_t count);

/*
 * Return file size.
 */
size_t fsize (FILE * f);

#endif /* __UTILS_H__ */
