/* 
 * readlinep - a utility function to safely read one line of input;
 * see readlinep.h for documentation.
 *
 * David Kotz, April 2016, 2017
 * CS50, January 2020
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "readlinep.h"

/* freadlinep - see readlinep.h for description */
char *freadlinep(FILE *fp) 
{
  // validate the parameter
  if (fp == NULL || feof(fp)) {
    return NULL;
  }

  // allocate buffer big enough for "typical" words/lines
  int len = 81;
  char *buf = calloc(len, sizeof(char));
  if (buf == NULL) {
    return NULL;	      // out of memory
  }

  // Read characters from file until newline or EOF, 
  // expanding the buffer when needed to hold more.
  int pos;
  char c;
  for (pos = 0; (c = fgetc(fp)) != EOF && (c != '\n'); pos++) {
    // We need to save buf[pos+1] for the terminating null
    // and buf[len-1] is the last usable slot, 
    // so if pos+1 is past that slot, we need to grow the buffer.
    if (pos+1 > len-1) {
      char *newbuf = realloc(buf, ++len);
      if (newbuf == NULL) {
      	free(buf);
      	return NULL;	      // out of memory
      } else {
	buf = newbuf;
      }
    }
    buf[pos] = c;
  }

  if (pos == 0 && c == EOF) {
    // no characters were read and we reached EOF
    free(buf);
    return NULL;
  } else {
    // pos characters were read into buf[0]..buf[pos-1].
    buf[pos] = '\0'; // terminate string
    return buf;
  }
}


#ifdef QUICKTEST
/* A simple unit test: 
 * open a file given as command-line argument, 
 * read it line by line,
 * and print each line surrounded by [brackets].
 */
int main(const int argc, char *argv[])
{
  if (argc != 2) 
    exit(1);

  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    printf("can't open %s\n", argv[1]);
    exit(2);
  }

  char *line;
  while ( (line = freadlinep(fp)) != NULL) {
    printf("[%s]\n", line);
    free (line);
  }
  fclose(fp);
}
#endif
