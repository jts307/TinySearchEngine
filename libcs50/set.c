/* 
 * set.c - set module, modeled after CS50 'bag' module
 *
 * see set.h for more information.
 *
 * Jacob Werzinsky, CS50, January 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "readlinep.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct setnode {
  const char *key;            // key that indentifies item	
  void *item;		      // pointer to data for this item
  struct setnode *next;	      // link to next node
} setnode_t;

/**************** global types ****************/
typedef struct set {
  struct setnode *head;	      // head of the list of items in set
} set_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static setnode_t *setnode_new(const char *key, void *item);

/**************** set_new() ****************/
set_t* set_new(void)
{
  set_t *set = count_malloc(sizeof(set_t));
  
  // error allocating memory for set
  if (set == NULL) {
    return NULL;
  } else {
    // initialize contents of the set structure
    set->head = NULL;
    return set;
  }
}

/**************** set_insert() ****************/
bool set_insert(set_t *set, const char *key, void *item)
{
  if ((set != NULL) && (key != NULL) && (item != NULL)) {
    // looping through set 
    for (setnode_t *node = set->head; node != NULL; node = node->next){
      // if key is found return false
      if (strcmp(key, node->key) == 0) {
        return false;
      }
    }
    // allocate a new node to be added to the list
    setnode_t *new = setnode_new(key, item);
    if (new != NULL) {
      // add it to the head of the list
      new->next = set->head;
      set->head = new;
      return true;
    }
  }
  return false;
#ifdef MEMTEST
  count_report(stdout, "After set_insert");
#endif
}


/**************** setnode_new ****************/
/* Allocate and initialize a setnode */
static setnode_t* setnode_new(const char *key, void *item)
{
  // allocating memory for string key and setnode	
  setnode_t *node = count_malloc(sizeof(setnode_t));
  char *key_copy = count_malloc(sizeof(strlen(key) + 1));

  if ((node == NULL) || (key_copy == NULL)) {
    // if error allocating memory for node or key string then return NULL
    return NULL;
  } else {
    // copying string to new pointer
    strcpy(key_copy, key);
    // setting node's values with values passed from the caller
    node->key = key_copy;    
    node->item = item;
    node->next = NULL;
    return node;
  }
}

/**************** set_find() ****************/
void *set_find(set_t *set, const char *key)
{
  // checking validity of passed pointers	
  if ((set != NULL) && (key != NULL)) {
    // looping through set
    for (setnode_t *node = set->head; node != NULL; node = node->next) {
      // if key is found return setnode's item
      if (strcmp(key, node->key) == 0) {
        return node->item;
      }
    } 
  } 
  return NULL;
}

/**************** set_print() ****************/
void set_print(set_t *set, FILE *fp,
               void (*itemprint)(FILE *fp, const char *key, void *item) )
{
  if (fp != NULL) {
    if (set != NULL) {
      fputc('{', fp);
      for (setnode_t *node = set->head; node != NULL; node = node->next) {
        // print this node
        if (itemprint != NULL) {  // print the node's item 
          (*itemprint)(fp, node->key, node->item); 
	  // print comma if not last node
	  if (node->next != NULL) {
	    fputc(',',fp);		  
	  }
        }	  
      }
      fputs("}\n", fp);
    } else {
      // print null if NULL set.
      fputs("(null)\n", fp);
    }
   }
}

/**************** set_iterate() ****************/
void set_iterate(set_t *set, void *arg,
                 void (*itemfunc)(void *arg, const char *key, void *item) )
{
  if ((set != NULL) && (itemfunc != NULL)) {
    // loop through set
    for (setnode_t *node = set->head; node != NULL; node = node->next) {
      // call itemfunc with arg on each item/key pair
      (*itemfunc)(arg, node->key, node->item); 
    }
  }
}

/**************** set_delete() ****************/
void set_delete(set_t *set, void (*itemdelete)(void *item) )
{
  if (set != NULL) {
    // loop through set
    for (setnode_t *node = set->head; node != NULL;) {	 
      // delete item if possible   
      if (itemdelete != NULL) {		    
        (*itemdelete)(node->item);	    
      }
      // delete node's key
      if (node->key != NULL) {		    
        count_free((char*)node->key);
      }
      setnode_t *next = node->next;	    // remember next node
      count_free(node);			    // free current node
      node = next;			    // move to next node
    }
    // free set itself
    count_free(set);
  }

#ifdef MEMTEST
  count_report(stdout, "End of set_delete");
#endif
}
