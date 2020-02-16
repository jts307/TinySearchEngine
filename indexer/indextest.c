/* 
 * indextest.c - tests the index data structure and its methods by saving data from an Index file
 * into an index struct and then rewriting the data to a new file using the created index.
 * usage: indextest [oldIndexFilename] [newIndexFilename]
 * input:
 * 	-oldIndexFilename: the path of the file that contains the index to be rewritten.
 *       index must be in the following format:
 *      	- one word per line, one line per word
 *      	- each line has the following format:
 *      		-word docId1 count1 docId2 count2...
 *	-newIndexFilename: the file the index will be rewitten to. Will be overwritten if the file
 *       already exists.	
 * output:
 * 	An index will be written to newIndexFilename in the format described in oldIndexFilename above.
 *      It also outputs progress indicators to standard out.	
 * Exit statuses:
 *   0 - success
 *   1 - Usage error
 *   2 - oldIndexFilename is not valid
 *   3 - newIndexFilename is not valid
 *   4 - Faliure to initialize index struct
 *   5 - Error loading index from oldIndexFilename  
 * Created by Jacob Werzinsky, CS50, 21 January 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "index.h"
#include "file.h"

int main(const int argc, const char *argv[]){

  int status=0;			// exit status    	
  int reqNumArgs=3; 		// required number of arguments
  FILE *oldIndexFile=NULL;	// original index list
  FILE *newIndexFile=NULL;	// file index will be rewritten to
  index_t *index=NULL;		// medium to store gathered info from files

  // checking numbers of arguments passed
  if (argc != reqNumArgs) {
      // if wrong number of arguments then return an error 
      fprintf(stderr, "Usage: indextest [oldIndexFilename] [newIndexFilename]\n");
      status++;
  } else {
      // checking if oldIndexFilename is readable
      if ((oldIndexFile=fopen(argv[2], "r")) == NULL) {
          // if not then return error
          fprintf(stderr, "oldIndexFilename must be a readable existing file.\n");
          status+=2;
      // checking if newIndexFilename is writable if it exists	  
      } else if ((newIndexFile=fopen(argv[3], "w")) == NULL) {
	  // if not then return error
	  fprintf(stderr, "newIndexFilename must be writable.\n");
	  status+=3;
      // initialize index structure
      } else if ((index = index_new(lines_in_file(oldIndexFile)*1.34)) == NULL) {
          // on error return
	  fprintf(stderr, "Could not initialize index struct\n");
	  status+=4;
      } else {
	  // load index from file
	  if (index_load(index, oldIndexFile) != 0) {
	    fprintf(stderr, "Error loading index from file");
	    status+=5;
	  }
	  // save the created index to the new file
	  index_save(index, newIndexFile);

	  // clean up, free memory
	  fclose(oldIndexFile);
	  fclose(newIndexFile);
	  index_delete(index);
      }
  }
  return status;
}
