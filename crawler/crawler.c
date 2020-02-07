/* 
 * crawler.c - 
 * usage: crawler 
 * input:
 * output:
 * Exit statuses:
 *          
 * Created by Jacob Werzinsky, CS50, 21 January 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "memory.h"
#include "webpage.h"
#include "bag.h"
#include "hashtable.h"
#include "pagedir.h"

int crawler(const char *seedURL, const char *pageDirectory, const int maxDepth);

int main(const int argc, const char *argv[]){

  int status=0;		// exit status    	
  int reqNumArgs=3; 	// required number of arguments

  // checking numbers of arguments passed
  if (argc != reqNumArgs) {
      // if wrong number of arguments then return an error 
      fprintf(stderr, "Usage: crawler seedURL pageDirectory maxDepth");
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
          fprintf(stderr, "seedURL is not internal or valid.");
          status+=3;
      // checking if pageDirectory is a valid and writable directory 
      } else if (!isValidDirectory(argv[2])) {
          // if not then return error
          fprintf(stderr, "pageDirectory is not a valid and writable directory.");
          status+=4;	    
      } else if (maxDepth < 0) {
	  // if not then return error
	  fprintf(stderr, "maxDepth must be nonnegative.");
	  status+=5;
      } else {
          // start a crawl from seedURL
          status = crawler(argv[1], argv[2], maxDepth);
      }
  }
  return status;
}

/*
 *
 *
*/
int crawler(const char *seedURL, const char *pageDirectory, const int maxDepth) {
  
  int status=0; 	           // exit status
  bag_t *toBeVisited=NULL; 	   // webpages that have not been crawled
  hashtable_t *visitedURLs=NULL;   // urls that have already been crawled
  webpage_t *seedWp=NULL;	   // webpage for seed URL
  webpage_t *currWp=NULL;	   // current webpage being crawled
  webpage_t *newWp=NULL; 	   // one of the next webpages to be crawled
  char *nextURL=NULL;		   // potential next URL to crawl
  int pos=0;			   // marker used in webpage_getNextURL()
  
  // initialize bag of webpages
  if ((toBeVisited = bag_new()) ==  NULL) {
      
      // if error then increment status, log error
      fprintf(stderr, "Error creating bag of webpages.");	  
      status+=6;

  // initialize hashtable of urls
  } else if ((visitedURLs = hashtable_new(500)) == NULL) {
      // if error then increment status, log error
      fprintf(stderr, "Error creating hashtable of URLs");
      status+=7;

  // making a webpage for seedURL marked with depth of 0 and NULL html.
  } else if ((seedWp = webpage_new((char*)seedURL, 0, NULL)) == NULL) {
      // if error then increment status, log error
      fprintf(stderr, "Error creating a webpage type for the seed URL.");
      status+=8;

  // inserting seedURL into visited URLS
  } else if (!(hashtable_insert(visitedURLs,seedURL, ""))) {
      // if error then increment status, log error
      fprintf(stderr, "Error inserting seedURL into visitedURLs hashtable.");
      status +=9;
  } else {
      // add seedURL to bag and hashtable to start
      bag_insert(toBeVisited,seedWp);

      // continuing to loop if there are pages to crawl
      while ((currWp = bag_extract(toBeVisited)) != NULL) {	
	
	// fetch HTML for current webpage
        if(!webpage_fetch(currWp)) {
          // if error, increment status and log it
	  fprintf(stderr, "There was an error fetching webpage html for %s", webpage_getURL(currWp));
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
	        
		// create webpage for URL
	        if ((newWp=webpage_new(nextURL, webpage_getDepth(currWp) + 1,NULL)) == NULL) {
		    // if error increment status, log it, and continue
                    fprintf(stderr, "Error creating webpage type for %s", nextURL);
		    status=12;

		// queue webpage for crawling 
		} else {
	            bag_insert(toBeVisited,newWp);
		}		
	      }
	    }	  
            free(nextURL);
          }
        }
	webpage_delete(currWp);
      } 
  }
  
  // freeing up memory
  bag_delete(toBeVisited, webpage_delete);
  hashtable_delete(visitedURLs, NULL);
  webpage_delete(seedWp); 

  return status;
}
