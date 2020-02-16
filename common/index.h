/* 
 * index.h - header file for index module
 *
 * An index is a mapping from words to (documentID, count) pairs, where 
 * a documentID is the name of a file in the page directory and the count
 * is the number of times a word appears in documentID. It does not store
 * words with less than three characters and lowercases all words before it
 * stores them.
 *
 * Jacob Werzinsky, CS50, February 2020
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

/**************** global types ****************/
typedef struct index index_t;

/**************** functions ****************/

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
 * 	- one line per word, one word per line
 * 	- each line has the format: word docID1 count1 docID2 count2...
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
 *   	0 - success
 *   	1 - function recieves NULL arguments
 *   	2 - There was an issue inserting at least one pair into the index  
 */ 
int index_load(index_t *index, FILE *fp);

#endif // __INDEX_H

