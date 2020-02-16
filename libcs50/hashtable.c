/* 
 * hashtable.c - 'hash table' module
 *
 * see hashtable.h for more information
 *
 *
 * Jacob Werzinsky, CS50, January 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memory.h"
#include "set.h"
#include "jhash.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
typedef struct hashtable {
  set_t **slots;
  const int *total_slots;
} hashtable_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** local functions ****************/
/* none */

/**************** hashtable_new() ****************/
hashtable_t *hashtable_new(const int num_slots)
{
  // error when slots 0 or less	
  if (num_slots <= 0) {
    return NULL;
  }	  
  // allocating memory for hash table
  hashtable_t *ht = count_malloc(sizeof(hashtable_t));
  if (ht == NULL) {
    return NULL; // error allocating hashtable
  } else {
    // allocating space for array of sets
    ht->slots = count_calloc(num_slots, sizeof(set_t*));
    if (ht->slots == NULL) {
      return NULL; // error allocating array of sets	    
    }
    // Initializing each set in the array of sets to an empty set
    for (int slot = 0; slot < num_slots; slot++) {
      ht->slots[slot] = set_new();
      if (ht->slots[slot] == NULL) {
        return NULL;  // error allocating memory for set
      }
    }
    // allocating memory and initializing int pointer used in hashtable struct
    int *total = count_malloc(num_slots * sizeof(int));
    if (total == NULL) {
      return NULL;  // error allocating memory
    }
    *total = num_slots;
    ht->total_slots = total;

    return ht;
  }
}

/**************** hashtable_insert() ****************/
bool hashtable_insert(hashtable_t *ht, const char *key, void *item)
{
  if ((ht != NULL) && (ht->slots != NULL) && (ht->total_slots != NULL) 
		  && (key != NULL) && (item != NULL)) {
    // using hash function to find out which set the key should go in.	  
    set_t *set = ht->slots[JenkinsHash(key, *ht->total_slots)];
    // inserting pair in that set
    return set_insert(set, key, item);
  }
  return false;

#ifdef MEMTEST
  count_report(stdout, "After hashtable_insert");
#endif
}

/**************** hashtable_find() ****************/
void *hashtable_find(hashtable_t *ht, const char *key)
{
  if ((ht != NULL) && (ht->slots != NULL) && (key != NULL) 
		  && (ht->total_slots != NULL)) {
    // using hash function to find out which set the key should be in.	  
    set_t *set = ht->slots[JenkinsHash(key, *ht->total_slots)]; 
    // searching that set for (key,item) pair
    return set_find(set, key);
  }
  return NULL;
}

/**************** hashtable_print() ****************/
void hashtable_print(hashtable_t *ht, FILE *fp, 
                     void (*itemprint)(FILE *fp, const char *key, void *item))
{
  if (fp != NULL) {
    if ((ht != NULL) && (ht->slots != NULL) && (ht->total_slots != NULL)) {
      // looping through sets in hashtable and printing them	    
      for (int slot=0; slot < *ht->total_slots; slot++) { 
        set_print(ht->slots[slot], fp, itemprint);	    
      }
    // print null when hashtable is null  
    } else {
      fputs("(null)\n", fp);
    }
  }
}

/**************** hashtable_iterate() ****************/
void hashtable_iterate(hashtable_t *ht, void *arg,
                       void (*itemfunc)(void *arg, const char *key, void *item) )
{		
  if ((ht != NULL) && (ht->slots != NULL) && 
		  (ht->total_slots != NULL) && (itemfunc != NULL)) {
    // looping through all the sets in the hashtable
    for (int slot = 0; slot < *ht->total_slots; slot++) {
      // iterate through each set and call itemfunc with arg on each (key,item) pair	    
      set_iterate(ht->slots[slot], arg, itemfunc); 
    }
  }
}

/**************** hashtable_delete() ****************/
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) )
{
  if (ht != NULL) {
    if ((ht->slots != NULL) && (ht->total_slots != NULL)) { 
      // freeing each set in hashtable	    
      for (int slot = 0; slot < *ht->total_slots; slot++) {
        set_delete(ht->slots[slot], itemdelete); 
      } 
      // freeing array that stores sets
      count_free(ht->slots);
      // freeing hashtable set count
      count_free((int*)ht->total_slots);
    }
    // freeing the hashtable itself
    count_free(ht);
  }
#ifdef MEMTEST
  count_report(stdout, "End of hashtable_delete");
#endif
}
