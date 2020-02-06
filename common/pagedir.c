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
int isValidDirectory(const char *pageDir)
{
  // dummy file to be created in directory	
  const char *fileName = ".crawler"; 

  // creating path inside directory for dummy file  
  const char *filePath = getFilePath(pageDir, fileName);

  // if dummy file is not created then the directory is invalid
  FILE *fp;
  if ((fp = fopen(filePath, "w")) == NULL) {
    count_free((char*)filePath);
    return 1;
  // else the directory is valid
  } else {
    fclose(fp);
    count_free((char*)filePath);
    return 0;
  }
}
/**************** webpageToPageFile() ****************/
void pageSaver(const char *pageDir, webpage_t *wp)
{
  // checking arguments are not NULL.
  assertp((char*)pageDir, "pageSaver gets NULL pageDir\n");
  assertp(wp, "pageSaver gets NULL wp\n");
  assertp(webpage_getURL(wp), "pageSaver gets NULL wp->url\n");
  assertp(webpage_getHTML(wp), "pageSaver gets NULL wp->html\n");
  
  // checking if saved depth is positive
  if (webpage_getDepth(wp) < 0) {
    // on error log it, return and continue	  
    fprintf(stderr, "The depth for a webpage is not positive. Webpage URL: %s\n", webpage_getURL(wp));
    return;
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
  }
  
  // writing url to first line of file
  if (fprintf(fp, "%s\n", webpage_getURL(wp)) < 0) {
    // on error log and continue	  
    fprintf(stderr, "Problem writing url to %s\n", filePath); 	  
  }

  // writing depth to second line of file
  if (fprintf(fp, "%d\n", webpage_getDepth(wp)) < 0) {
    // on error log and continue
    fprintf(stderr, "Problem writing depth to %s\n", filePath); 
  }

  // writing html to third line onwards of file
  if (fprintf(fp, "%s", webpage_getHTML(wp)) < 0) {
    // on error log and continue
    fprintf(stderr, "Problem writing HTML to %s\n", filePath); 
  }
  count_free((char*)filePath);
  fclose(fp);
}

/**************** webpageToPageFile() ****************/
static char* getFilePath(const char *pageDir, const char *fileName)
{       	
  // checking that arguments are not NULL.
  assertp((char*)pageDir, "getFilePath gets NULL pageDir\n");
  assertp((char*)fileName, "getFilePath gets NULL fileName\n");

  // allocating space for filePath pointer  
  char *filePath = count_malloc(strlen(pageDir) + strlen(fileName) + 2);
  assertp(filePath, "Failure to allocate space for filePath pointer.\n");

  // creating directory path string from directory and file strings
  strcpy(filePath, pageDir);
  strcat(filePath, "/");
  strcat(filePath, fileName);
  return filePath;  
}
