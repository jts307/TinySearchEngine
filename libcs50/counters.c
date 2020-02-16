/* 
 * counters.c - 'counters' module, modeled after CS50 'bag' module
 *
 * see counters.h for more information.
 *
 * Jacob Werzinsky, CS50, January 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include "counters.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct counternode {
  int count;		      // counter for this item	
  int key;	      // key for this item
  struct counternode *next;   // link to next node
} counternode_t;

/**************** global types ****************/
typedef struct counters {
  struct counternode *head;	      // head of the list of items in counters
} counters_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see counters.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static counternode_t *counternode_new(const int key, int count);

/**************** counters_new() ****************/
counters_t* counters_new(void)
{
  counters_t *ctrs = count_malloc(sizeof(counters_t));
  
  // error allocating memory for counters
  if (ctrs == NULL) {
    return NULL; 
  } else {
    // initialize contents of counters structure
    ctrs->head = NULL;
    return ctrs;
  }
}

/**************** counters_add() ****************/
int counters_add(counters_t *ctrs, const int key)
{
  if ((ctrs != NULL) && (key >= 0)) {

    // looping through counters
    for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
      // check if key is among counter nodes
      if (node->key == key) {
        // incrementing count if so
	node->count++;
	return node->count;
      }	
    }
    // allocate a new node to be added to the list
    counternode_t *new = counternode_new(key, 0);
    new->count++;

    if (new != NULL) {
      // add it to the head of the list
      new->next = ctrs->head;
      ctrs->head = new;
      return ctrs->head->count;
    } 
  }
  // ignore and return on error
  return 0;
  

#ifdef MEMTEST
  count_report(stdout, "After counters_add");
#endif
}


/**************** counternode_new ****************/
/* Allocate and initialize a counternode */
static counternode_t* counternode_new(const int key, int count)
{
  counternode_t *node = count_malloc(sizeof(counternode_t));

  if (node == NULL) {
    // error allocating memory for node
    return NULL;
  } else {  
    //setting node's variables
    node->key = key;
    node->count = count;
    node->next = NULL;
    return node;
  }
}

/**************** counters_get() ****************/
int  counters_get(counters_t *ctrs, const int key)
{
  if ((ctrs != NULL) && (key >= 0)) {
    // looping through counters	  
    for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
      // if the specified counter key is found return its count
      if (node->key == key) {
	      return node->count;
      }
    }
  } 
  return 0;
}

/**************** counters_set *****************/
bool counters_set(counters_t *ctrs, const int key, const int count)
{
  if ((ctrs != NULL) && (key >= 0) && (count >= 0)) {
    // loop through counters
    for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
      // if node with key is found then set count
      if (node->key == key) {
        node->count=count;
	return true;
      }  
    }
    // allocate a new node to be added to the list
    counternode_t *new = counternode_new(key, count);
    // checking if memory was allocated properly for new node
    if (new != NULL) {
      // add it to the head of the list
      new->next = ctrs->head;
      ctrs->head = new;
      return true;
    } 
  }
  return false;
}

/**************** counters_print() ****************/
void counters_print(counters_t *ctrs, FILE *fp)
{
  if (fp != NULL) {
    if (ctrs != NULL) {
      fputc('{', fp);
      for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
        // print this node's key and count
	fprintf(fp, "%d=%d", node->key, node->count);
	if (node->next != NULL) {
          fputs(",", fp);
	}
      }
      fputs("}\n", fp);
    } else {
      // if counters is null then print null.	    
      fputs("(null)", fp);
    }
  }
}

/**************** counters_iterate() ****************/
void counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg,
			const int key, const int count))
{
  if ((ctrs != NULL) && (itemfunc != NULL)) {
    // call itemfunc with arg, and each node's key and count
    for (counternode_t *node = ctrs->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->key, node->count); 
    }
  }
}

/**************** counters_delete() ****************/
void counters_delete(counters_t *ctrs)
{
  if (ctrs != NULL) {
    // freeing all counternodes
    for (counternode_t *node = ctrs->head; node != NULL; ) {
      counternode_t *next = node->next;	    // remembering next node
      count_free(node);			    // freeing current node
      node = next;			    // moving on to next node
    }
    // free counters itself
    count_free(ctrs);
  }

#ifdef MEMTEST
  count_report(stdout, "End of counters_delete");
#endif
}
