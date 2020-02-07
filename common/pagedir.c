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

/**************** file-local global variables ****************/
static int id = 0;  // id of file that will be created in the page Directory. 

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
  FILE *fp;
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
/**************** webpageToPageFile() ****************/
int pageSaver(const char *pageDir, webpage_t *wp)
{
  // exit status
  int status=0;

  if (pageDir == NULL) {
    // if NULL return error
    fprintf(stderr, "pageSaver gets NULL pageDir");
    status++;
    return status;
  }

  // checking arguments other than pageDir are not NULL.
  // Upon a NULL argument, it is logged and function returns
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
    // converting int id to char*
    numDigits = ceil(log10(id));
    stringId = count_malloc((numDigits + 1)*sizeof(char));
    assertp(stringId, "Failure to allocate space for stringId pointer\n");
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
    count_free((char*)filePath);
    fclose(fp);
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
  
  // returning NULL if failure to allocate memory
  if (filePath == NULL) {
    fprintf(stderr, "Failure to allocate space for filePath pointer.\n");
    return NULL;
  }
  // creating directory path string from directory and file strings
  strcpy(filePath, pageDir);
  strcat(filePath, "/");
  strcat(filePath, fileName);
  return filePath;  
}
