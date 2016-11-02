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

/*
 * Inflate one member then stop.  As the function iterates through the
 * stream, up to max_in bytes are read, inflated, and up to max_out
 * bytes are written.  The callback function procMember is invoked after
 * each inflate, giving the caller the opportunity to, for instance,
 * write the inflated data to a file.
 */
extern int inflateMember (z_stream *z, FILE *f, unsigned int max_in, unsigned int max_out, void (*procMember) (z_stream *, int, void *), void *userPtr);

/*
 * Inflate n members then stop but do not invoke a callback function,
 * thus discarding the inflated data.  This function is useful for
 * seeking to the beginning of the next nth member in the stream.
 */
extern int dismissMembers (z_stream *z, FILE *f, unsigned int max_in, unsigned int max_out, unsigned int *undismissed);

/*
 * Like dismissMembers, but inflate one member only then stop.
 */
extern int dismissMember (z_stream *z, FILE *f, unsigned int max_in, unsigned int max_out);

/* 
 * Unpack multi-member gzip file. Each output file contains one member only.
 */
extern int gzunpack (char *input_file, char *output_dir);

/*
 * Print usage message.
 */
void help (void);

/* 
 * Copy compressed chunk from file to another, from current position with
 * bytes count, size.
 */
void fcopy (FILE *inf, FILE *outf, size_t size);

/*
 * Return file size.
 */
size_t fsize (FILE * f);
#endif /* __GZMULTI_H__ */
