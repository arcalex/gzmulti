#ifndef __GZMULTI_H__
#define __GZMULTI_H__

#include <stdio.h>
#include <zlib.h>

#define CHUNK_FIRST 0
#define CHUNK_MIDDLE 1
#define CHUNK_LAST 2
#define CHUNK_FIRST_LAST 3

extern int inflateMember (z_stream *z, FILE *f, unsigned int max_in, unsigned int max_out, void (*procMember) (z_stream *, int, void *), void *userPtr);
extern int dismissMembers (z_stream *z, FILE *f, unsigned int max_in, unsigned int max_out, unsigned int *undismissed);
extern int dismissMember (z_stream *z, FILE *f, unsigned int max_in, unsigned int max_out);

#endif /* __GZMULTI_H__ */
