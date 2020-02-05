/* 
 * pagedir.c - 'pagedir' module, contains operations that write to the page directory.
 *
 * see pagedir.h for more information.
 *
 *
 * Jacob Werzinsky Winter 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "webpage.h"
#include "memory.h"

/**************** file-local global variables ****************/
// current file number

/**************** local types ****************/
/* none */

/**************** global types ****************/
/* none */

/**************** global functions ****************/
/* that is, visible outside this file */
/* see pageDir.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
/* none */

/**************** isValidDirectory() ****************/
int isValidDirectory(const char *pageDir)
{
  // dummy file to be created in directory	
  const char *fileName = ".crawler"; 

  // creating path inside directory for dummy file  
  char *filePath = count_malloc(strlen(pageDir) + strlen(fileName) + 2);
  strcpy(filePath, pageDir);
  strcat(filePath, "/");
  strcat(filePath, fileName);

  // if dummy file is not created then the directory is invalid
  FILE *fp;
  if ((fp = fopen(filePath, "w")) == NULL) {
    count_free(filePath);
    return 1;
  // else the directory is valid
  } else {
    fclose(fp);
    count_free(filePath);
    return 0;
  }
}

/**************** webpageToPageFile() ****************/
//void* webpageToPageFile(char *pageDir, webpage_t *wp, const char *id)
//{

//}
