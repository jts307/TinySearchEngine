/* 
 * readlinep - a utility function to safely read one line of input,
 * of any length, and return a pointer to a null-terminated string.
 *
 * David Kotz, April 2017
 * CS50, January 2020
 */

#ifndef __READLINEP_H__
#define __READLINEP_H__

#include <stdio.h>

/* readline, freadline:
 *
 * Read a line from the file into a null-terminated string,
 * and return a pointer to it; caller must later free() the pointer.
 * The string returned includes NO newline, and a terminating null.
 * Returns empty string if an empty line is read.
 * Returns NULL if error, or EOF reached.
 */
extern char *freadlinep(FILE *fp);
static inline char *readlinep(void) { return freadlinep(stdin); }

#endif //  __READLINEP_H__
