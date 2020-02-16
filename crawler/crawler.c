/* 
 * crawler.c - Crawls webpages starting from a seed URL. It explores the seed URL which is 
 * marked with a depth of 0 and stores its depth, url and html in a file with a unique id. 
 * Then all urls on the webpage of the seed URL are explored as well marked with a depth of 
 * 1 while also storing their information as well into files. This continues, adding 1 to depth
 * each time, until all urls are exhausted or maxDepth is reached.
 * usage: crawler [seedURL] [pageDirectory] [maxDepth]
 * input: 
 *   seedURL - url the crawling process begins at. Must contain html and be internal to 
 *   'http://old-www.cs.dartmouth.edu/'.
 *   pageDirectory - the directory where webpage information in the form of files are stored.
 *   maxDepth - maximum depth before crawl process is ended.
 * output:
 *   A series files, each that contain the url of a webpage on the first line, depth on the second,
 *   and a webpage's html from the third line onward. Each file has a unique number id.
 * Exit statuses:
 *   0 - success
 *   1 - Usage error
 *   2 - non-numerical max Depth
 *   3 - not a valid and/or internal URL
 *   4 - not a valid or writable pageDirectory
 *   5 - negative maxDepth
 *   6 - error creating bag type of webpages
 *   7 - error creating hashtable type of urls
 *   8 - error creating a webpage type for seed URL
 *   9 - error inserting seed URL into hashtable
 *  10 - error fetching html for a url
 *  11 - error writing webpage to a file
 *  12 - error creating webpage type for a URL other than seed URL
 *          
 * Created by Jacob Werzinsky, CS50, 21 January 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "memory.h"
#include "webpage.h"
#include "bag.h"
#include "hashtable.h"
#include "pagedir.h"

int crawler(const char *seedURL, const char *pageDirectory, const int maxDepth);
void webprint(FILE *fp, void *wp);
void urlprint(FILE *fp, const char *url, void *placeholder);


int main(const int argc, const char *argv[]){

  int status=0;		// exit status    	
  int reqNumArgs=4; 	// required number of arguments
  
  // checking numbers of arguments passed
  if (argc != reqNumArgs) {
      // if wrong number of arguments then return an error 
      fprintf(stderr, "Usage: crawler [seedURL] [pageDirectory] [maxDepth]\n");
      status++;
  } else {
      // converting maxDepth argument to int	  
      char* check;    
      const int maxDepth = strtod(argv[3], &check);

      // return error if no numbers were in passed argument
      if (check == argv[3]) {
          status+=2;
	  fprintf(stderr, "Error: maxDepth must be numerical.\n");	

      // checking if seedURL is internal and valid (i.e. can be parsed and contains html) 
      } else if (!IsInternalURL((char*)argv[1])) {
          // if not then return error
          fprintf(stderr, "seedURL is not internal and/or valid.\n");
          status+=3;

      // checking if pageDirectory is a valid and writable directory 
      } else if (!isValidDirectory(argv[2])) {
          // if not then return error
          fprintf(stderr, "pageDirectory is not a valid and writable directory.\n");
          status+=4;
      // checking if max Depth is nonnegative	  
      } else if (maxDepth < 0) {
	  // if not then return error
	  fprintf(stderr, "maxDepth must be nonnegative.\n");
	  status+=5;
      } else {
          // start a crawl from seedURL
          status = crawler(argv[1], argv[2], maxDepth);
      }
  }
  return status;
}

/* Crawls webpages that are internal and valid. Internal meaning they start with 
 * 'http://old-www.cs.dartmouth.edu/' and valid meaning they are accessible and 
 * have html code. 
 *
 * Parameters:
 *   seedURL: url crawler starts out at and branches out from.
 *   pageDirectory: directory were information about the webpages will be stored.
 *   maxDepth: maximum depth before crawl process is ended. 
 * returns:
 *   status - zero indicates success, non-zero is failure.
 */
int crawler(const char *seedURL, const char *pageDirectory, const int maxDepth) {
  
  int status=0; 	           // exit status
  bag_t *toBeVisited=NULL; 	   // webpages that have not been crawled
  hashtable_t *visitedURLs=NULL;   // urls that have already been crawled
  webpage_t *seedWp=NULL;	   // webpage for seed URL
  webpage_t *currWp=NULL;	   // current webpage being crawled
  webpage_t *newWp=NULL;           // one of the next webpages to be crawled
  char *nextURL=NULL;		   // potential next URL to crawl
  char *cpySeedURL=NULL;           // copy of seed URL passed to webpage type
  int pos=0;			   // marker used in webpage_getNextURL()

  // allocating space for string copy
  cpySeedURL = count_malloc(strlen(seedURL)*sizeof(char)+1);
  assertp(cpySeedURL, "Error allocating memory for cpySeedURL.");

  // create copy of seed URL
  strcpy(cpySeedURL, seedURL);
   
  // initialize bag of webpages
  if ((toBeVisited = bag_new()) ==  NULL) {
      // if error then increment status, log error
      fprintf(stderr, "Error creating bag of webpages.\n");	  
      status+=6;
  // initialize hashtable of urls
  } else if ((visitedURLs = hashtable_new(200)) == NULL) {
      // if error then increment status, log error
      fprintf(stderr, "Error creating hashtable of URLs\n");
      status+=7;
  // making a webpage for seedURL marked with depth of 0 and NULL html.
  } else if ((seedWp = webpage_new((char*)cpySeedURL, 0, NULL)) == NULL) {
      // if error then increment status, log error
      fprintf(stderr, "Error creating a webpage type for the seed URL.\n");
      status+=8;
  // inserting seedURL into visited URLS
  } else if (!(hashtable_insert(visitedURLs,seedURL, ""))) {
      // if error then increment status, log error
      fprintf(stderr, "Error inserting seedURL into visitedURLs hashtable.\n");
      status +=9;
  } else {
      // add seedURL to bag and hashtable to start
      bag_insert(toBeVisited,seedWp);
    
      // continuing to loop if there are pages to crawl
      while ((currWp = bag_extract(toBeVisited)) != NULL) {
	    
	// progress indicators for testing purposes
	// printf("\n");
        // hashtable_print(visitedURLs, stdout, urlprint);
	// printf("\n");
	// bag_print(toBeVisited, stdout, webprint);

	// fetch HTML for current webpage
        if(!webpage_fetch(currWp)) {
          // if error, increment status and log it
	  fprintf(stderr, "There was an error fetching webpage html for %s\n", webpage_getURL(currWp));
	  status=10;    
        }
    
	// writing an output file for current webpage
        if (pageSaver(pageDirectory,currWp) != 0) {
          // increment status (error already logged within pageSaver)
	  status=11;
	}
	// explore links on webpage if it is within the maxDepth
	if (webpage_getDepth(currWp) < maxDepth) {
              
	// looping through links on webpage
       	pos=0;
        while ((nextURL = webpage_getNextURL(currWp, &pos)) != NULL) {
	// normalize URL and check if it is internal
	if (IsInternalURL(nextURL)) {
	         
	  // check if URL has been visited
	  if (hashtable_insert(visitedURLs,nextURL, "")) {
	        
            // copy of nextURL for use in webpage type
            char *cpyNextURL = count_malloc(strlen(nextURL)*sizeof(char)+1);
            assertp(cpyNextURL, "Error allocating memory for cpyNextURL");
            strcpy(cpyNextURL, nextURL);
	    // create webpage for URL
	    if ((newWp=webpage_new(cpyNextURL, webpage_getDepth(currWp) + 1,NULL)) == NULL) {
	       // if error increment status, log it, free memory and continue
               fprintf(stderr, "Error creating webpage type for %s\n", nextURL);
	       status=12;
	       count_free(cpyNextURL);
	    // queue webpage for crawling 
            } else {
	        bag_insert(toBeVisited,newWp);
	    }			
	  }  
	}
        free(nextURL);
      }
    }
    // progress indicators for testing purposes
    // printf("\n");
    // hashtable_print(visitedURLs, stdout, urlprint);
    // printf("\n");
    // bag_print(toBeVisited, stdout, webprint);
    webpage_delete(currWp);	
  } 
 }
 // freeing up memory
 bag_delete(toBeVisited, webpage_delete);
 hashtable_delete(visitedURLs, NULL); 

  return status;
}

/*
 * Used for testing, prints out the information in a webpage type
 */
void webprint(FILE *fp, void *wp) {
  fprintf(fp, "url: %s", webpage_getURL(wp));
  fprintf(fp, " depth: %d", webpage_getDepth(wp));
}
/*
 * Used for testing, prints out a url in a hashtable
 */
void urlprint(FILE *fp, const char *url, void *placeholder) {
  fprintf(fp, "url: %s", url);
}	
