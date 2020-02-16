/* 
 * pagedir.c - 'pagedir' module, contains operations that write to the page directory.
 *
 * see pagedir.h for more information.
 *
 * Jacob Werzinsky Winter 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "webpage.h"
#include "memory.h"
#include "file.h"

/**************** file-local global variables ****************/
static int id = 1;  // id of file that will be created in the page Directory. 

/**************** local types ****************/
/* none */

/**************** global types ****************/
/* none */

/**************** global functions ****************/
/* that is, visible outside this file */
/* see pageDir.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static char* getFilePath(const char *pageDir, const char *fileName);

/**************** functions **********************/

/**************** isValidDirectory() ****************/
bool isValidDirectory(const char *pageDir)
{	
  // checking if pageDir is NULL, exiting if it is
  assertp((char*)pageDir, "isValidDirectory gets NULL pageDir");

  // dummy file to be created in directory	
  const char *fileName = ".crawler"; 
  
  // creating path inside directory for dummy file  
  const char *filePath = getFilePath(pageDir, fileName);
  assertp((char*)filePath, "Error creating filePath string for directory.");

  // if dummy file is not created then the directory is invalid
  FILE *fp=NULL;
  if ((fp = fopen(filePath, "w")) == NULL) {
    count_free((char*)filePath);
    return false;
  // else the directory is valid
  } else {
    fclose(fp);
    count_free((char*)filePath);
    return true;
  }
}

/**************** isCrawlerDirectory() ****************/
bool isCrawlerDirectory(const char *pageDir)
{
  // checking if pageDir is NULL, exiting if it is
  assertp((char*)pageDir, "isCrawlerDirectory gets NULL pageDir");

  // dummy file name that should be in directory	
  const char *fileName = ".crawler"; 
  
  // creating path for dummy file  
  const char *filePath = getFilePath(pageDir, fileName);
  assertp((char*)filePath, "Error creating filePath string for isCrawlerDirectory.");

  // if dummy file is not in the directory then it is invalid
  FILE *fp=NULL;
  if ((fp = fopen(filePath, "r")) == NULL) {
    count_free((char*)filePath);
    return false;
  // else the directory is valid
  } else {
    fclose(fp);
    count_free((char*)filePath);
    return true;
  }
}

/**************** pageSaver() ****************/
int pageSaver(const char *pageDir, webpage_t *wp)
{
  // exit status
  int status=0;

  // checking if arguments are not NULL.
  // Upon a NULL argument, it is logged and function returns
  if (pageDir == NULL) {
    fprintf(stderr, "pageSaver gets NULL pageDir");
    status++;
    return status;
  }
  if (wp == NULL) {
    fprintf(stderr, "pageSaver gets NULL wp\n");
    status = 2;
    return status;
  }
  if (webpage_getURL(wp) == NULL) {
    fprintf(stderr, "pageSaver gets NULL wp->url\n");
    status = 3;
    return status;
  }
  if (webpage_getHTML(wp) ==  NULL) {
    fprintf(stderr, "pageSaver gets NULL wp->html\n");
    status = 4;
    return status;
  }
  
  // checking if saved depth is positive
  if (webpage_getDepth(wp) < 0) {
    // on error log it, return and continue	  
    fprintf(stderr, "The depth for a webpage is not positive. Webpage URL: %s\n", webpage_getURL(wp));
    status = 5;
    return status;
  }

  int numDigits=0;       // number of digits that file id has
  char *stringId=NULL;   // id converted to a char* type
  char *filePath=NULL;   // path file will be created at
  FILE *fp=NULL;	 

  // finding file id that is availible for use 
  while (1) {
    // converting int id to char
    numDigits = ceil(log10(id));
    stringId = (char*)count_malloc((numDigits+2)*sizeof(char)); 
    assertp(stringId, "Failure to allocate space for stringId pointer");
    sprintf(stringId, "%d", id);
    
    // creating file path string from file id
    filePath = getFilePath(pageDir, stringId);

    if (filePath == NULL) {
      // log error and return
      fprintf(stderr,"Error with creating filePath string for file id.");
      status = 6;
      return status; 
    }
    count_free(stringId);

    // move on to next file id
    id++; 
    
    // if file does not exist then its availible for use.
    if ((fp = fopen(filePath, "r")) == NULL) {
      break;
    }
    fclose(fp);
    count_free(filePath);
  }

  // create and write page file
  if ((fp = fopen(filePath, "w")) == NULL) {
    // on error log and continue	  
    fprintf(stderr, "Problem creating %s\n", filePath);
    status = 7;
  }
  
  // writing url to first line of file
  if (fprintf(fp, "%s\n", webpage_getURL(wp)) < 0) {
    // on error log and continue	  
    fprintf(stderr, "Problem writing url to %s\n", filePath); 	  
    status = 7;
  }

  // writing depth to second line of file
  if (fprintf(fp, "%d\n", webpage_getDepth(wp)) < 0) {
    // on error log and continue
    fprintf(stderr, "Problem writing depth to %s\n", filePath); 
    status = 7;
  }

  // writing html to third line onwards of file
  if (fprintf(fp, "%s", webpage_getHTML(wp)) < 0) {
    // on error log and continue
    fprintf(stderr, "Problem writing HTML to %s\n", filePath); 
    status = 7;
  }
  count_free((char*)filePath);
  fclose(fp);
  return status;
}

/**************** webpageLoad() ****************/
webpage_t *webpageLoad(const char *pageDir, int id)
{
  FILE *fp=NULL;	// file pointer for numbered file
  webpage_t *wp=NULL;	// webpage created out of numbered file
  char *url=NULL;	// url from numbered file
  char *depth=NULL;	// depth from numbered file
  char *html=NULL;	// html from numbered file
  int idepth=-1;	// depth converted to int type
  char *filePath=NULL;	// path file is at
  int numDigits;	// number of digits id has
  char *stringId=NULL;  // conversion of id to a char* type

  if (id > 0 && pageDir != NULL) { 
    // converting fileId to a string
    numDigits = ceil(log10(id));
    stringId = (char*)count_malloc((numDigits+2)*sizeof(char)); 
    assertp(stringId, "Failure to allocate space for stringId pointer\n");
    sprintf(stringId, "%d", id);
    
    if ((filePath = getFilePath(pageDir, stringId)) != NULL) {
  
      if ((fp = fopen(filePath, "r")) != NULL) {
        // parsing contents of file into strings
        url = freadlinep(fp);
        depth = freadlinep(fp);
        html = freaduntil(fp, NULL);
  
        if (depth != NULL) {
          // converting depth to int	    
          idepth = strtod(depth, NULL);
          count_free(depth);
        }  

        if (url != NULL && html != NULL && idepth != -1) {
          // creating webpage for numbered file
          wp = webpage_new(url, idepth, html);
	  if (wp == NULL) {
	    fprintf(stderr, "There was an error allocating memory for webpage struct\n");
	  }
        } else {
	  fprintf(stderr, "Error reading from file using webpageLoad\n");
	}
	// freeing memory if any failures
        if (wp == NULL && url != NULL) {
	  count_free(url);
	}
	if (wp == NULL && html != NULL) {
	  count_free(html);
	}
        fclose(fp); 
      }
      count_free(filePath);
    } else {
      fprintf(stderr, "Error: Could not create file path using getFilePath\n");
    }
    count_free(stringId);
  } else {
    fprintf(stderr, "webpageLoad gets NULL argument\n");
  }
  // wp is NULL on error
  return wp;
}

/* Used to get create a file path inside a directory.
 * Parameters:
 *   pageDir - directory path 
 *   fileName - name of file wanting to create path for 
 * Returns:
 *   A path to the passed file inside the passed directory
 */
/**************** getFilePath() ****************/
static char* getFilePath(const char *pageDir, const char *fileName)
{       	
  // return NULL if directory name is NULL.
  if (pageDir == NULL) {
    fprintf(stderr, "getFilePath gets NULL pageDir\n");
    return NULL;
  }
  // allocating space for filePath pointer  
  char *filePath = count_malloc(strlen(pageDir) + strlen(fileName) + 2);
  
  // exiting if failure to allocate memory
  assertp(filePath, "Failure to allocate space for filePath pointer.\n");
  
  // creating directory path string from directory and file strings
  strcpy(filePath, pageDir);
  strcat(filePath, "/");
  strcat(filePath, fileName);
  return filePath;  
}
