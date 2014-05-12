#ifndef __GZMULTI_H__
#define __GZMULTI_H__

#include <stdio.h>
#include <zlib.h>
#include <string.h>
#include <assert.h>

extern int inflateMember (FILE *f, z_stream *z, Bytef *next_out, uLong avail_out);

#endif /* __GZMULTI_H__ */
