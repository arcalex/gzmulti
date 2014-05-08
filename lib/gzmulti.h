#ifndef __GZMULTI_H__
#define __GZMULTI_H__

#include <zlib.h>

#define GZM_BUF_SIZE 128 * 1024

extern FILE *gzmopen (const char *path, const char *mode);
extern int *gzmclose (FILE *fp);

extern int gzminflate (FILE *fp, Bytef *obuf, uInt obuf_len);

#endif  /* __GZMULTI_H__ */
