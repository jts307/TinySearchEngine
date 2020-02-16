# CS50 Lab 4, Lab 5
## Jacob Werzinsky, CS50 Winter 2020

# CS50 Tiny Search Engine (TSE) common library

This library contains modules that have functions that are shared between crawler, indexer and querier.

## Usage

To build `common.a`, run `make`. 

To clean up, run `make clean`.

## Overview

 * `pagedir` - functhat that involve writing files to the pageDirectory
 * `word` - functions that involve modifying char* types.
 * `index` - the **index** data structure

## pagedir

The pagedir module handles operations that involve writing files to the pageDirectory. This includes dummy files 
and webpage files created by crawler.

### Usage

The *pagedir* module, defined in `pagedir.h` and implemented in `pagedir.c`:

```c
/* Checks if a directory exists and is writable by attempting to write 
 * '.crawler' dummy file in directory. Note that .crawler remains in 
 * directory after function is done.
 * parameters:
 *   pageDir - pointer to the path of directory to be tested
 * returns:
 *   int with value of 0 if directory exists and is writable, 1 otherwise.
 * When pageDir is NULL or failed memory allocation, 
 * then an error is written to stderr and it exits with positive status.
 */
int isValidDirectory(const char *pageDir);

/* Takes a webpage type and writes its information into a page file with the
 * following format:
 *      Line 1: webpage URL
 *      Line 2: webpage depth
 *      Line 3 and onward: webpage HTML
 * parameters:
 *   pageDir - directory page file will be saved to
 *   webpage - pointer to webpage type whose information will be written to a file.
 * returns:
 *   returns 0 on success and positive value on any error.
 *   Error statuses:
 *      1 - NULL pageDir
 *      2 - NULL wp
 *      3 - NULL wp->url
 *      4 - NULL wp->html
 *      5 - Negative wp->depth
 *      6 - failed to create filePath String
 *      7 - Error creating or writing to page file
 *      8 - Error allocating memory for stringId         
 */
int pageSaver(const char *pageDir, webpage_t *wp);

/* Takes the nonnegative id of a file within the page directory and returns 
 * a webpage type that contains information obtained from the file, i.e. 
 * a url, html and depth.
 * parameters:
 *      pageDir - pointer to a string of the path of a directory
 *      id - the number name of a file within the pageDir
 * returns:
 *      pointer to the webpage type on success and NULL on any error.
 * */
webpage_t *webpageLoad(const char *pageDir, int id);
```

### Assumptions

The design spec did not specify how we should handle the case where there are already numbered files within
the directory passed by the caller. For example, the case where someone wants to use the same directory for
different calls of crawler with various seed URLs. So I assumed that a user would prefer to keep any numbered
files within the passed directory. This mainly pertains to the pageSaver() function.

## word

The word module handles operations that involve char* types.

### Usage

The *word* module, defined in `word.h` and implemented in `word.c`:

```c
/* Changes all alphabetic characters in a string to lower case.
 * parameters:
 *   word - the string to be edited. Must be mutable.
 * returns:
 *   0 - on sucess
 *   1 - on failure caused by null word
 */
int normalizeWord(char *word);
```

### Assumptions

It assumes that the caller will not pass a string whose memory location cannot be modified (I thought this was fine since this is also assumed by normalizeURL).

## index

The index module defines the index struct and handles operations on it.

### Usage

The *index* module, defined in `index.h` and implemented in `index.c`:

```c
/**************** index_new() ****************/
/* Create a new (empty) index.
 *
 * Caller provides:
 *   number of slots to be used for the hashtable used in the index (must be > 0).
 * We return:
 *   pointer to the new index; return NULL if error.
 * We guarantee:
 *   index is initialized empty.
 * Caller is responsible for:
 *   later calling index_delete.
 */
index_t *index_new(const int num_slots);

/**************** index_insert() ****************/
/* Insert a word and (documentID, count) pair into the index
 * Caller provides:
 *   valid pointer to index, valid string for word, valid int for .
 * We return:
 *   false if any parameter is NULL, or error;
 *   true iff new pair was inserted.
 * Notes:
 *   The key string is copied for use by the index; that is, the module
 *   is responsible for allocating memory for a copy of the key string, and
 *   later deallocating that memory; thus, the caller is free to re-use or
 *   deallocate its word string after this call.
 */
bool index_insert(index_t *index, const char *word, const int docId, const int wordCount);

/**************** index_find() ****************/
/* Return the item associated with the given key.
 *
 * Caller provides:
 *   valid pointer to index, valid string for word, valid int for docID
 * We return:
 *   count associated with a word and documentID, if found;
 *   0 if any error or word and docID pair not found.
 * Notes:
 *   the index is unchanged by this operation.
 */
int index_find(index_t *index, const char *word, const int docId);
/**************** index_print() ****************/
/* Print the whole index; provide the output file.
 * Caller provides:
 *   valid pointer to index.
 *   FILE open for writing,
 * We print:
 *   nothing, if NULL fp or NULL index.
 *   one line per word where each line is in the format: (word, (docID=count), (docID=count),...)
 */
void index_print(index_t *index, FILE *fp);

/**************** index_delete() ****************/
/* Delete index, i.e. free up any memory taken up by it
 *
 * Caller provides:
 *   valid index pointer
 * We do:
 *   if index==NULL, do nothing.
 *   free all the word strings, the hashtable inside the index struct, and the index itself.
 */
void index_delete(index_t *index);

/**************** index_save() ****************/
/* Save a index to a file. 
 *
 * Caller provides:
 *   valid index pointer, valid open FILE pointer
 * We save:
 *   Saves each word and (documentID, count) pair to passed FILE
 *   The file would have the following format:
 *      - one line per word, one word per line
 *      - each line has the format: word docID1 count1 docID2 count2...
 */
void index_save(index_t *index, FILE *fp);

/**************** index_load() ****************/
/* Load a index from a file. 
 *
 * Caller provides:
 *   valid index pointer, valid open FILE pointer
 *   The file must have the following format:
 *      - one line per word, one word per line
 *      - each line has the format: word docID1 count1 docID2 count2...
 * We do:
 *   Load each word and (documentID, count) pair found in passed file into the passed index
 * We return:
 *   An exit status which indicates success or failure:
 *      0 - success
 *      1 - function recieves NULL arguments
 *      2 - There was an issue inserting at least one pair into the index  
 */
int index_load(index_t *index, FILE *fp);
```

## Compilation

To compile common archive, do: `make common.a`.

To compile pagedir module, do: `make pagedir.o`.

To compile word module, do: `make word.o`.

To compile index module, do: `make index.o`
