#ifndef __GZMULTI_H__
#define __GZMULTI_H__

#include <stdio.h>
#include <zlib.h>

#define CHUNK_FIRST 0
#define CHUNK_MIDDLE 1
#define CHUNK_LAST 2
#define CHUNK_FIRST_LAST 3

/**
 * Inflate one member then stop.  As the function iterates through the
 * stream, up to max_in bytes are read, inflated, and up to max_out
 * bytes are written.  The callback function procMember is invoked after
 * each inflate, giving the caller the opportunity to, for instance,
 * write the inflated data to a file.
 */
extern int inflateMember (z_stream *z, FILE *f, size_t max_in, size_t max_out, void (*procMember) (z_stream *, int, void *), void *userPtr);

/**
 * Inflate n members then stop but do not invoke a callback function,
 * thus discarding the inflated data.  This function is useful for
 * seeking to the beginning of the next nth member in the stream.
 */
extern int dismissMembers (z_stream *z, FILE *f, size_t max_in, size_t max_out, uInt *undismissed);

/**
 * Like dismissMembers, but inflate one member only then stop.
 */
extern int dismissMember (z_stream *z, FILE *f, size_t max_in, size_t max_out);

/** 
 * Unpack multi-member GZIP file.  Each output file contains one member
 * only.
 */
extern int gzunpack (char *input_file, char *output_dir);

/**
 * Blind insert of member_file at the given offset in input_file.
 * 
 * @param input_file input multi-member GZIP file
 * @param output_file output multi-member GZIP file
 * @param member_file file containing the new member to be inserted
 * @param off_str offset to insert at
 * @return count of bytes inserted
 */
extern size_t gzinsert (char *input_file, char *output_file, char *member_file, char *off_str);

/**
 * Blind append of member_file at the end of input_file.
 * 
 * @param input_file input multi-member GZIP file
 * @param output_file output multi-member GZIP file
 * @param member_file file containing the new member to be appended
 * @return count of bytes appended
 */
extern size_t gzappend (char *input_file, char *output_file, char *member_file);

/**
 * Discard n members starting from the given offset (which must be the
 * beginning of a valid GZIP member), and replace with member_file.
 * 
 * @param input_file input multi-member GZIP file
 * @param output_file output multi-member GZIP file
 * @param off_str offset to replace at
 * @param undismissed number of members to be dismissed, such that
 * after the function call, undismissed contains the count of members
 * that could not be dismissed (in case no more members to dismiss)
 * @param member_file file containing the new member to be replaced
 * @return count of bytes deleted
 */
extern size_t gzreplace (char *input_file, char *output_file, char *off_str, uInt *undismissed, char *member_file);

/** 
 * Discard n members starting from the given offset (which must be the
 * beginning of a valid GZIP member).
 * 
 * @param input_file input multi-member GZIP file
 * @param output_file output multi-member GZIP file
 * @param off_str offset to delete at
 * @param undismissed number of members to be dismissed, such that
 * after the function call, undismissed contains the count of members
 * that could not be dismissed (in case no more members to dismiss)
 * @return count of bytes deleted
 */
extern size_t gzdelete (char *input_file, char *output_file, char *off_str, uInt *undismissed);

#endif /* __GZMULTI_H__ */
