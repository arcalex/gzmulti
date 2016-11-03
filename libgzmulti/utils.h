#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

#define INPUT_CHUNK 4 * 1024
#define OUTPUT_CHUNK 8 * 1024

/* 
 * Copy compressed chunk from file to another, from current position with
 * bytes count, size.
 */
void fcopy (FILE *inf, FILE *outf, size_t size);

/*
 * Return file size.
 */
size_t fsize (FILE * f);

#endif /* __UTILS_H__ */
