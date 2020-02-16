/* 
 * indexer.c - reads the document files previously produced by the TSE crawler, builds
 * an index struct out of what is read, and writes this index to a file. The index maps 
 * each word in the files to (documentId, count) pairs, where a documentId is the name of
 * a file and the count is how many times a word appears in that file.
 * usage: indexer [pageDirectory] [indexFilename]
 * input:
 *    - pageDirectory: a directory previously crawled by crawler that contains
 *      the files to be read.
 *    - indexFilename: the file to write the index to
 * output:
 *   Produces a file with the words, documentIds, and counts in the following format:
 *     - one word per line, one line per word
 *     - where each line has the format:
 *     	   word documentId1 count1 documentId2 count2... 
 * Exit statuses:
 *   0 - success
 *   1 - Usage error
 *   2 - not a valid pageDirectory
 *   3 - indexFilename is not writable
 *   4 - Failure to initialize index struct
 *   5 - Error inserting into index      
 * Created by Jacob Werzinsky, CS50, 21 January 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memory.h"
#include "webpage.h"
#include "pagedir.h"
#include "index.h"

int index_build(index_t *index, const char *pageDirectory);

int main(const int argc, const char *argv[]){

  int status=0;		// exit status    	
  int reqNumArgs=3; 	// required number of arguments
  FILE *indexFile=NULL;	// index list will be written here
  index_t *index=NULL;	// medium to store gathered info from crawler files
  			// until it is written to a file

  // checking numbers of arguments passed
  if (argc != reqNumArgs) {
      // if wrong number of arguments then return an error 
      fprintf(stderr, "Usage: indexer [pageDirectory] [indexFilename]\n");
      status++;
  } else {
      // checking if pageDirectory is a valid and readable directory 
      if (!isCrawlerDirectory(argv[1])) {
          // if not then return error
          fprintf(stderr, "pageDirectory is not a valid readable Crawler directory.\n");
          status+=2;
      // checking if indexFilename is writable if it exists	  
      } else if ((indexFile=fopen(argv[2], "w")) == NULL) {
	  // if not then return error
	  fprintf(stderr, "indexFilename must be writable.\n");
	  status+=3;
      // initialize index structure
      } else if ((index = index_new(900)) == NULL) {
          // on error return
	  fprintf(stderr, "Could not initialize index struct\n");
	  status+=4;
      } else {
	  // create index from contents of pageDirectory
          status = index_build(index, argv[1]);

	  // save the created index to a file
	  index_save(index, indexFile);

	  // clean up, free memory
	  fclose(indexFile);
	  index_delete(index);
      }
  }
  return status;
}

/* It seaches through the files within the pageDirectory and keeps a count of how
 * many times every word within those files appears in each file. These counts
 * are stored within the index structure.
 * Parameters:
 *   index: an ideally empty index to store the info obtained from the pageDirectory
 *   pageDirectory: directory previously crawled by crawler
 * returns:
 *   status - zero indicates success, non-zero is any failure.
 */
int index_build(index_t *index, const char *pageDirectory) {
  webpage_t *wp=NULL;	// a webpage created out of a file's contents
  char *word=NULL;      // current word being read from a pageDirectory file
  int docId=1;		// current document name being read
  int pos=0;		// tracks place in html for webpage_getNextWord
  int status=0;		// exit status

  // looping and creating a webpage struct for each file in pageDirectory
  while ((wp = webpageLoad(pageDirectory, docId)) != NULL) {
    
    pos=0;
    // looping through the words in the html for each webpage struct
    while ((word = webpage_getNextWord(wp, &pos)) != NULL) {
      
      // used for testing purposes	    
      printf("Attempting to insert (%s, %d) into index.\n", word, docId);

      // Incrementing word and document id pair's count by 1 in index 
      if (!index_insert(index, word, docId, index_find(index, word, docId) + 1)) {
	// on error log it and continue
        fprintf(stderr, "Trouble inserting (%s, %d) into index\n", word, docId);
        status=5;
      }
      count_free(word);
    }
    // used for testing purposes
    printf("Index after scanning %s/%d\n", pageDirectory, docId);
    index_print(index, stdout);

    // go to next file
    docId++;	  
    webpage_delete(wp);
  }
  return status;
}
