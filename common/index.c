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
#include <string.h>
#include <stdbool.h>
#include "word.h"
#include "hashtable.h"
#include "counters.h"
#include "file.h"
#include "memory.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
/* none */

/**************** global types ****************/
typedef struct index {
  hashtable_t *ht;
} index_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see set.h for comments about exported functions */

/**************** local functions ****************/
void index_line_print(void *fp, const char *word, void *ctrs);
void counts_print(void *fp, const int docId, const int count);
void pair_print(FILE *fp, const char *word, void *ctrs);
void counters_delete_2(void *ctrs);

/**************** index_new() ****************/
index_t *index_new(const int num_slots)
{
  index_t *index = count_malloc(sizeof(index_t));
  
  // checking if memory was allocated properly
  assertp(index,"Error allocating memory for index\n");
  
  // allocating memory for and initializing index's hashtable
  index->ht = hashtable_new(num_slots);
  assertp(index->ht, "Error allocating memory for index's hashtable\n");
  return index; 
}

/**************** index_insert() ****************/
bool index_insert(index_t *index, char *word, const int docId, const int wordCount)
{
  counters_t *wordCounters=NULL;	// the docId counts of a particular word
  
  // checking arguments
  if ((index != NULL) && (index->ht != NULL) && (word != NULL) && (docId > 0)) { 
    // making sure word has 3 or more characters
    if (strlen(word) < 3) {
      fprintf(stderr, "'%s' is less than three characters\n", word);
      return false;
    }

    // changing word to all lowercase
    if (normalizeWord(word) == 1) {
      // if error log and return
      fprintf(stderr, "Trouble normalizing '%s'\n", word);
      return false;
    }

    // attempting to find word in index
    if ((wordCounters = hashtable_find(index->ht, word)) == NULL) {
      // if it is not in index then create a counters for it
      wordCounters = counters_new();
    
      // add docId to the counters
      counters_set(wordCounters, docId, wordCount);
    
      // attempt to add (word, counters) pair to index
      if (!hashtable_insert(index->ht, word, wordCounters)) {
        // if it was not added then an error must have occurred
        fprintf(stderr, "There was an issue inserting (word=%s,docId=%d) to the index.\n",
			word, docId);
	return false;
      }
    // if it is in index then add docId to existing counters object for word
    } else {
      if (!counters_set(wordCounters, docId, wordCount)) {
        fprintf(stderr, "There was an error incrementing the counters struct for %s", word);
	return false;
      }
    }
    return true;
  }
  fprintf(stderr, "index_insert gets NULL argument.\n");
  return false;
#ifdef MEMTEST
  count_report(stdout, "After index_insert");
#endif
}

/**************** index_find() ****************/
int index_find(index_t *index, const char *word, const int docId)
{
  if ((index != NULL) && (word != NULL) && (index->ht != NULL)) {
    // searching index for word count
    return counters_get(hashtable_find(index->ht, word), docId);
  }
  fprintf(stderr, "index_find gets NULL argument\n");
  return 0;
}

/**************** index_print() ****************/
void index_print(index_t *index, FILE *fp)
{
  if ((index != NULL) && (fp != NULL) && (index->ht != NULL)) {
    hashtable_print(index->ht, fp, pair_print);
  }
}

/**************** index_delete() ****************/
void index_delete(index_t *index)
{
  if (index != NULL) {
    hashtable_delete(index->ht, counters_delete_2);
  }
#ifdef MEMTEST
  count_report(stdout, "End of index_delete");
#endif
}

/**************** index_save() ****************/
void index_save(index_t *index, FILE *fp)
{
  // going through index and printing each word and counters pair
  if ((index != NULL) && (fp != NULL && index->ht != NULL)) { 
    hashtable_iterate(index->ht, fp, index_line_print);
  } else {
    fprintf(stderr, "index_save gets NULL argument\n");
  }
}

/**************** index_load() ****************/
int index_load(index_t *index, FILE *fp)
{
  char *word=NULL;	// a word to be added to the index
  int docId=0;		// docId to be added to the index
  int wordCount=0;	// the count of times word appears in docId
  int status=0;		// exit status

  if ((fp != NULL) && (index != NULL) && (index->ht != NULL)) {
    // reading first word of each line
    while ((word = freadwordp(fp)) != NULL) {
    
      // reading each subsequent docId and word count
      while (fscanf(fp, "%d %d ", &docId, &wordCount) == 2) {
        
	// on error log it and continue
        if (index_insert(index, word, docId, wordCount) != 0) {
          fprintf(stderr, "Problem inserting word: %s and docId: %d into the index", word, docId);
          status=2;	
	}
      }
      count_free(word);    
    }
  // log error and increment status
  } else {
      fprintf(stderr, "index_load gets NULL arguments");
      status++;
  }
  return status;
}

/**************** pairPrint() ****************/
/* Prints a word and counters struct pair to a file stream
 * in a human friendly syntax. 
 * can/should be passed to hashtable_print
 * Parameters:
 *   word - string word to be printed
 *   ctrs - counters struct to be printed
 *   fp - file stream that will have pair printed to 
 * Returns:
 *   nothing
 * if fp is NULL nothing is printed, if word is NULL it is not printed
 * if ctrs is NULL it is not printed
 */
void pair_print(FILE *fp, const char *word, void *ctrs)
{
  if (fp != NULL) {	
    fputs("(word=",fp);
    if (word != NULL) {
      fprintf(fp, "%s", word);
    } 
    fputs(",counters=", fp);
    counters_print(ctrs, fp);
    fputs(")",fp);
  }
}

/**************** counts_print() ****************/
/* Prints a docId and word occurance count from counters struct to a file stream, 
 * can/should be passed to counters_iterate
 * Parameters:
 *   fp - pointer to file stream to be written to
 *   docId - key from counters struct
 *   count - count for the key from counters struct
 * Returns:
 *   nothing
 * if fp is null nothing is printed
 */
void counts_print(void *fp, const int docId, const int count)
{
  if (fp != NULL) {
    if (fprintf(fp, " %d %d ", docId, count) < 0) {
      fprintf(stderr, "There was an issue printing docId: %d\n", docId);
    }
  }
}

/**************** index_line_print() ****************/
/* Prints a word and each docId counter associated with that word in 
 * a single line of a file stream, can/should be passed to hashtable_iterate
 * Parameters:
 *   word - string word to be printed
 *   ctrs - docId counters to be printed
 *   fp - file stream that will have line printed to 
 * Returns:
 *   nothing
 * if fp is null nothing is printed, if word is NULL it is not printed,
 * if ctrs is null it is not printed
 */
void index_line_print(void *fp, const char *word, void *ctrs)
{
  if (fp != NULL) {	
    // printing word to file
    if (word != NULL) {
      if (fprintf(fp, "%s", word) < 0) {
        fprintf(stderr, "There was an issue printing word: %s\n", word);
      }
    }
    // iterating through counters to print each docId counter
    counters_iterate(ctrs, fp, counts_print);
    if (fprintf(fp, "\n") < 0) {
      fprintf(stderr, "There was an issue printing to passed file.\n");
    }
  }
}

/**************** counters_delete_2() ****************/
/* Frees memory occupied by a counters struct. 
 * can/should be passed to hashtable_delete
 * Parameters:
 *   ctrs - counters struct to be freed
 * Returns:
 *   nothing
 */
void counters_delete_2(void *ctrs)
{
  counters_delete(ctrs);
}
