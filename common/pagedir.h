/* 
 * pagedir.h - header file for 'pagedir' module
 * 
 * The pagedir module has functions that handle processes that write to the page directory.
 * 
 * Jacob Werzinsky, Winter 2020
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "webpage.h"

/**************** global types ****************/
/* none */

/**************** functions ****************/

/* Checks if a directory exists and is writable by attempting to write 
 * '.crawler' dummy file in directory. Note that .crawler remains in 
 * directory after function is done.
 * parameters:
 *   pageDir - pointer to the path of directory to be tested
 * returns:
 *   true if directory exists and is writable, false otherwise.
 * When pageDir is NULL or failed memory allocation, 
 * then an error is written to stderr and it exits with positive status.
 */
bool isValidDirectory(const char *pageDir);

/* Checks if a directory exists, is readable and is crawler produced by 
 * attempting to read '.crawler' dummy file from directory. 
 * parameters:
 *   pageDir - pointer to the path of directory to be tested
 * returns:
 *   true if directory is valid, false otherwise.
 * When pageDir is NULL or failed memory allocation, 
 * then an error is written to stderr and it exits with positive status.
 */
bool isCrawlerDirectory(const char *pageDir);

/* Takes a webpage type and writes its information into a page file with the
 * following format:
 *	Line 1: webpage URL
 *	Line 2: webpage depth
 *	Line 3 and onward: webpage HTML
 * parameters:
 *   pageDir - directory page file will be saved to
 *   webpage - pointer to webpage type whose information will be written to a file.
 * returns:
 *   returns 0 on success and positive value on any error.
 *   Error statuses:
 *      1 - NULL pageDir
 *   	2 - NULL wp
 *   	3 - NULL wp->url
 *   	4 - NULL wp->html
 *   	5 - Negative wp->depth
 *   	6 - failed to create filePath String
 *   	7 - Error creating or writing to page file
 *   	8 - Error allocating memory for stringId 	 
 */
int pageSaver(const char *pageDir, webpage_t *wp);

/* Takes the nonnegative id of a file within the page directory and returns 
 * a webpage type that contains information obtained from the file, i.e. 
 * a url, html and depth.
 * parameters:
 * 	pageDir - pointer to a string of the path of a directory
 * 	id - the number name of a file within the pageDir
 * returns:
 * 	pointer to the webpage type on success and NULL on any error.
 * */
webpage_t *webpageLoad(const char *pageDir, int id);

/* Takes a document id and a pageDirectory and returns the url of the document
 * specified by the document id.
 * parameters:
 * 	pageDir- pointer to a string of the path of a directory
 * 	id - the number name of a file within the pageDir
 * returns:
 * 	A string containing the url within the specified document.
*/
const char *getPageURL(const char *pageDir, int id);

#endif // __PAGEDIR_H
