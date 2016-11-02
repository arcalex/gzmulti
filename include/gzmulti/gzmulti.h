#ifndef __GZMULTI_H__
#define __GZMULTI_H__

#include <stdio.h>
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <assert.h>

#define CHUNK_FIRST 0
#define CHUNK_MIDDLE 1
#define CHUNK_LAST 2
#define CHUNK_FIRST_LAST 3

#define INPUT_CHUNK 4 * 1024
#define OUTPUT_CHUNK 8 * 1024

extern int inflateMember (z_stream *z, FILE *f, unsigned int max_in, unsigned int max_out, void (*procMember) (z_stream *, int, void *), void *userPtr);
extern int dismissMembers (z_stream *z, FILE *f, unsigned int max_in, unsigned int max_out, unsigned int *undismissed);
extern int dismissMember (z_stream *z, FILE *f, unsigned int max_in, unsigned int max_out);


extern int gzunpack (char *input_file, char *output_dir);

void help (void);
void fcopy (FILE *inf, FILE *outf, size_t size);
size_t fsize (FILE * f);
#endif /* __GZMULTI_H__ */
