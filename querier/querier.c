/* 
 * querier.c - 
 * usage: querier [pageDirectory] [indexFilename]
 * input:
 * 	- pageDirectory: pathname of a directory produced by the Crawler
 * 	- indexFilename: pathname of a file produced by the Indexer
 * output:
 * 	
 * Exit statuses:
 *   0 - success
 *   1 - Usage Error
 *   2 - 
 *   3 - 
 *   4 - 
 *   5 - 
 * Created by Jacob Werzinsky, CS50, 21 February 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "memory.h"
#include "pagedir.h"
#include "index.h"
#include "file.h"
#include "word.h"

int fileno(FILE *stream);
static void prompt(void);
char **clean_input(char *input); 

int main(const int argc, const char *argv[])
{

  int status=0;			// exit status    	
  int reqNumArgs=3; 		// required number of arguments
  FILE *indexFile=NULL;		// index file to be read
  index_t *index=NULL;		// index will be loaded here for reading
  char *input=NULL;		// query to be searched
  char **cleanedInput=NULL;	// cleaned query to be searched

  // checking numbers of arguments passed
  if (argc != reqNumArgs) {
      // if wrong number of arguments then return an error 
      fprintf(stderr, "Usage: querier [pageDirectory] [indexFilename]\n");
      status++;
  } else {
      // checking if pageDirectory is a valid and readable directory 
      if (!isCrawlerDirectory(argv[1])) {
          // if not then return error
          fprintf(stderr, "pageDirectory is not a valid readable Crawler directory.\n");
          status+=2;
      // checking if indexFilename is readable	  
      } else if ((indexFile=fopen(argv[2], "r")) == NULL) {
	  // if not then return error
	  fprintf(stderr, "indexFilename must be readable.\n");
	  status+=3;
      // initialize index structure
      } else if ((index = index_new(lines_in_file(indexFile)*1.34)) == NULL) {
          // on error return
	  fprintf(stderr, "Could not initialize index struct\n");
	  status+=4;
      } else {
	  // create index from contents of indexFilename
          if (index_load(index, indexFile) != 0) {
            fprintf(stderr, "Error loading index from file\n");
            status+=5;
          }
	  fclose(indexFile);

	  // read input from stdin until EOF
	  while (!feof(stdin)) {
 
	    // if interactive user print prompt
	    prompt();
	    if ((input=readlinep()) != NULL) {
	      // clean and parse query input into proper syntax
	      if ((cleanedInput=clean_input(input)) != NULL) {
		  
		  for (int j = 0; j < 5; j++) {
    		    printf("%s\n", cleanedInput[j]);
  		  }

	          count_free(cleanedInput);
	      }
              free(input);
            // on error, log it and continue		    
	    }  
	  }
	  // free memory
	  index_delete(index);
      }
  }
  return status;
}

/* 'Cleans' a string, i.e. it seperates a string (char *) into a series of words (char **)
 * based on a space delimiter and makes sure the string follows proper query syntax as 
 * described in the documentation.
 * Parameters:
 * 	-input: string to be parsed
 * returns:
 * 	-words: array of words string will be parsed into
 * 	-NULL, if there is any improper syntax or any error (other than a memory allocation 
 * 	 error, in which case the query program exits).
 */
char **clean_input(char *input) 
{
  char** words;		  // array of words to be created
  int count=0;		  // place in array a word will be placed
  char *rest=input;	  // getting a pointer to start of string
  bool pervWasOp=false;   // was the pervious word parsed 'and' or 'or'

  // max # of pointers is the max # of words
  words = count_calloc(strlen(input)/2+1, sizeof(char*));
  assertp(words, "Problem allocating memory for words array.\n");

  // splitting into words 
  while (*rest != '\0') {

    // starting address of a word
    char *start; 

    // seek the start of a word
    for (start = rest; !isalpha(*start) && *start != '\0'; start++) {
     
      // if non-alphabetic and non-space input is detected then
      // return error
      if ((!isalpha(*start)) && (!isspace(*start))) {
        fprintf(stderr, "Error: Input must contain only spaces and alphabetic characters.\n");
	count_free(words);
	return NULL;
      }
    }
    // seek the end of the word
    for (rest = start; isalpha(*rest) && *rest != '\0'; rest++) {  
    }

    // insert null character at end of a word 
    if (*rest != '\0') {
      // if non-alphabetic and non-space input is detected then
      // return error
      if (!isspace(*rest)) {
        fprintf(stderr, "Error: Input must contain only spaces and alphabetic characters.\n");
	count_free(words);
	return NULL;
      }  
      *rest++ = '\0';
    }
    if (*start != '\0') {
      // lowercase word 
      if (normalizeWord(start)) {
        // on error, return NULL
	fprintf(stderr, "Error: Could not normalize word within passed input.\n");
	count_free(words);
	return NULL;
      }
      // if `and` or `or` operators are encountered then checking for 
      // potential syntax errors
      if ((strcmp(start, "and") == 0) || (strcmp(start, "or") == 0)) {
          // if first word then syntax error
          if (count == 0) {
 	    fprintf(stderr, "Error: '%s' operator cannot be first\n", start);
	    count_free(words);
	    return NULL;
	  }
	  // if this comes after another operator then
	  // its a syntax error
	  if (pervWasOp) {
	    fprintf(stderr, "Error: 'and' and 'or' operators cannot follow each other.\n");
	    count_free(words);
	    return NULL;
	  }	
	  pervWasOp=true;
      } else {
      	  pervWasOp=false;
      }
      // insert word into words array 
      words[count++] = start;
    }
  }
  // syntax error if last word is an operator
  char *last=words[count-1];
  if ((strcmp(last, "and") == 0) || (strcmp(last, "or") == 0)) {
      fprintf(stderr, "Error: '%s' operator cannot be last\n", last);
      count_free(words);
      return NULL;
  } else {
      return words;
  }
}

/* Prints out a prompt if recieving input from an interactive user.
 * Provided by CS50 staff
*/ 
static void prompt(void)
{
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}
