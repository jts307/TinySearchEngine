/* 
 * index.c - 'index' module
 *
 * see index.h for more information
 *
 *
 * Jacob Werzinsky, CS50, February 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

/**************** global types ****************/
typedef struct index {
  hashtable_t *ht;
} index_t;

/**************** functions ****************/
/**************** index_new() ****************/
hashtable_t *index_new(const int num_slots);

/**************** index_insert() ****************/
int index_insert(index_t *index, const char *word, const int *docId, const int wordCount);

/**************** index_find() ****************/
int *index_find(index_t *index, const char *word, const int docId);

/**************** index_print() ****************/
void *index_print(index_t *index, FILE *fp);

/**************** index_delete() ****************/
void index_delete(index_t *index);

/**************** index_save() ****************/
void *index_save(index_t *index, FILE *fp);

/**************** index_load() ****************/
int *index_load(index_t *index, FILE *fp);
