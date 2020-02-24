/* 
 * querier.c - Answers queries from stdin based on the passed crawler directory and index file.
 *	       Queries must have the syntax described in the DESIGN.md for this program. 
 * usage: querier [pageDirectory] [indexFilename]
 * input:
 * 	- pageDirectory: pathname of a directory produced by the Crawler
 * 	- indexFilename: pathname of a file produced by the Indexer
 *	- Queries to standard input
 * output:
 * 	- answers to queries
 * Exit statuses:
 *   0 - success
 *   1 - Usage Error
 *   2 - 
 *   3 - 
 *   4 - 
 *   5 - 
 * Created by Jacob Werzinsky, CS50, 21 February 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "counters.h"
#include "memory.h"
#include "pagedir.h"
#include "index.h"
#include "file.h"
#include "word.h"

// stores two counters_t structs
// local to this program
typedef struct multiCounters {
  counters_t *ctrs1;
  counters_t *ctrs2;
} multi_counters_t;

// used to store document and their
// respective scores, local to this program
typedef struct document {
  int id;
  int score;
} document_t;



int fileno(FILE *stream);
static void prompt(void);
char **clean_input(char *input); 
counters_t *calculate_scores(index_t *index, char **words, int start, int end);
counters_t *or_counters(counters_t *ctrs1, counters_t *ctrs2);
counters_t *and_counters(counters_t *ctrs1, counters_t *ctrs2);
void sum_counters(void *ctrs, const int docId, const int count);
void minimum_counters(void *ctrs, const int docId, const int count);
void copy_counters(void *ctrs, const int docId, const int count);
void num_counters(void *total, const int docId, const int count);
void sort_counters(void *result, const int docId, const int count);

int main(const int argc, const char *argv[])
{

  int status=0;			   // exit status    	
  int reqNumArgs=3; 	 	   // required number of arguments
  FILE *indexFile=NULL;		   // index file to be read
  index_t *index=NULL;		   // index will be loaded here for reading
  char *input=NULL;	 	   // query to be searched
  char **cleanedInput=NULL;  	   // cleaned query to be searched
  counters_t *scores=NULL;	   // contains docIds and their scores for a query
  int total=0;			   // total number of documents matching a query
  document_t **sortedScores=NULL;  // ranked list of documents matching a query

  // checking numbers of arguments passed
  if (argc != reqNumArgs) {
      // if wrong number of arguments then return an error 
      fprintf(stderr, "Usage: querier [pageDirectory] [indexFilename]\n");
      status++;
  } else {
      // checking if pageDirectory is a valid and readable directory 
      if (!isCrawlerDirectory(argv[1])) {
          // if not then return error
          fprintf(stderr, "pageDirectory is not a valid readable Crawler directory.\n");
          status+=2;
      // checking if indexFilename is readable	  
      } else if ((indexFile=fopen(argv[2], "r")) == NULL) {
	  // if not then return error
	  fprintf(stderr, "indexFilename must be readable.\n");
	  status+=3;
      // initialize index structure
      } else if ((index = index_new(lines_in_file(indexFile)*1.34)) == NULL) {
          // on error return
	  fprintf(stderr, "Could not initialize index struct\n");
	  fclose(indexFile);
	  status+=4;
      // create index from contents of indexFilename
      } else if (index_load(index, indexFile) != 0) {
          fprintf(stderr, "Error loading index from file\n");
          fclose(indexFile);
          index_delete(index);
	  status+=5;
      } else {
	  fclose(indexFile);

	  // read input from stdin until EOF
	  while (!feof(stdin)) {
	    // if interactive user print prompt
	    prompt();

	    // getting query input
	    if ((input=readlinep()) != NULL) {
	      // clean and parse query input into proper syntax
	      if ((cleanedInput=clean_input(input)) != NULL) {
		printf("Query: ");

		// printing cleaned query input
		for (int i = 0; strcmp(cleanedInput[i],"\0") != 0; i++) {
		  printf("%s ", cleanedInput[i]);
		}
		printf("\n");
                	
		if ((scores=calculate_scores(index, cleanedInput, 0, -1)) != NULL) {
		    
		  total=0;	
		  counters_iterate(scores, &total, num_counters); 
		    
		  if (total==0) {
		      printf("No documents match.\n");
		  } else {
		      sortedScores=count_calloc(total, sizeof(document_t));
		      assertp(sortedScores, "Error allocating memory for array of document_t.\n");

		      counters_iterate(scores, sortedScores, sort_counters);

		      printf("Matches %d documents (ranked):\n", total);
		      for (int i=0; i<total; i++) {
      		      
		        int id=sortedScores[i]->id;   
		        printf("score %4d doc %4d: %s\n", sortedScores[i]->score, id, 
		      	          getPageURL(argv[1],id));
		        count_free(sortedScores[i]);
		      }
		      printf("-----------------------------------------------\n");

		      count_free(sortedScores);
		  }
		  if ((counters_get(scores, 0) == 1)) {
          	          counters_delete(scores); 
		  }
		} 
	        count_free(cleanedInput);
	      }
              free(input);		    
	    }  
	  }
	  // free memory occupied by index
	  index_delete(index);
      }
  }
  return status;
}

/* 'Cleans' a string, i.e. it seperates a string (char *) into a series of words (char **)
 * based on a space delimiter and makes sure the string follows proper query syntax as 
 * described in the documentation.
 * Parameters:
 * 	-input: string to be parsed
 * returns:
 * 	-words: array of words string will be parsed into, where "\0" is the last element of the array 
 * 	-NULL, if there is any improper syntax or any error (other than a memory allocation 
 * 	 error, in which case the query program exits).
 */
char **clean_input(char *input) 
{
  char** words;		  // array of words to be created
  int count=0;		  // place in array a word will be placed
  char *rest=input;	  // getting a pointer to start of string
  bool pervWasOp=false;   // was the pervious word parsed 'and' or 'or'

  // max # of pointers is the max # of words, 
  // add one more for terminating character 
  words = count_calloc(strlen(input)/2+2, sizeof(char*));
  assertp(words, "Problem allocating memory for words array.\n");

  // splitting into words 
  while (*rest != '\0') {

    // starting address of a word
    char *start; 

    // seek the start of a word
    for (start = rest; !isalpha(*start) && *start != '\0'; start++) {
     
      // if non-alphabetic and non-space input is detected then
      // return error
      if ((!isalpha(*start)) && (!isspace(*start))) {
        fprintf(stderr, "Error: Input must contain only spaces and alphabetic characters.\n");
	count_free(words);
	return NULL;
      }
    }
    // seek the end of the word
    for (rest = start; isalpha(*rest) && *rest != '\0'; rest++) {  
    }

    // insert null character at end of a word 
    if (*rest != '\0') {
      // if non-alphabetic and non-space input is detected then
      // return error
      if (!isspace(*rest)) {
        fprintf(stderr, "Error: Input must contain only spaces and alphabetic characters.\n");
	count_free(words);
	return NULL;
      }  
      *rest++ = '\0';
    }
    if (*start != '\0') {
      // lowercase word 
      if (normalizeWord(start)) {
        // on error, return NULL
	fprintf(stderr, "Error: Could not normalize word within passed input.\n");
	count_free(words);
	return NULL;
      }
      // if `and` or `or` operators are encountered then checking for 
      // potential syntax errors
      if ((strcmp(start, "and") == 0) || (strcmp(start, "or") == 0)) {
          // if first word then syntax error
          if (count == 0) {
 	    fprintf(stderr, "Error: '%s' operator cannot be first\n", start);
	    count_free(words);
	    return NULL;
	  }
	  // if this comes after another operator then
	  // its a syntax error
	  if (pervWasOp) {
	    fprintf(stderr, "Error: 'and' and 'or' operators cannot follow each other.\n");
	    count_free(words);
	    return NULL;
	  }	
	  pervWasOp=true;
      } else {
      	  pervWasOp=false;
      }
      // insert word into words array 
      words[count++] = start;
    }
  }
  // syntax error if last word is an operator
  char *last=words[count-1];
  if ((strcmp(last, "and") == 0) || (strcmp(last, "or") == 0)) {
      fprintf(stderr, "Error: '%s' operator cannot be last\n", last);
      count_free(words);
      return NULL;
  } else {
      words[count] = "\0";
      return words;
  }
}

/*
 *
 *
 */
counters_t *calculate_scores(index_t *index, char **words, int start, int end) 
{
  // either of these being null would be unexpected behavior
  // but if this does happen then exit with non-zero status
  assertp(index, "calculate_score gets NULL index\n");
  assertp(words, "calculate_score gets NULL words\n");

  // both point to recursively returned counters
  counters_t *ctrs1=NULL;
  counters_t *ctrs2=NULL;

  // result of an operation
  counters_t *result=NULL;

  // checking if words is empty
  if (strcmp(words[0],"\0") != 0) {
    // base case, if one word then find counters struct for that word
    if ((start==end) || (strcmp(words[start+1], "\0") == 0)) {
      if ((ctrs1=index_find_ctrs(index, words[start])) == NULL) {
        ctrs1=counters_new();
	assertp(ctrs1, "Error allocating memory for ctrs1\n");
	counters_set(ctrs1, 0, 1);
      }
      return ctrs1;
    }

    // loop through words list from 'start' to 'end' indexes
    int i=start;
    while ((i != end) && (strcmp(words[i],"\0") != 0)) {
      // checking for 'or' operator
      if (strcmp(words[i], "or") == 0) {
	// getting 'or's operands
	ctrs1 = calculate_scores(index, words, start,i-1);
	ctrs2 = calculate_scores(index, words, i+1, end);
       
	// performing 'or' operation
	result = or_counters(ctrs1,ctrs2);

	// freeing used counters if not part of index
	if ((counters_get(ctrs1, 0) == 1) && (ctrs1 != NULL)) {
	  counters_delete(ctrs1);
	}
	if ((counters_get(ctrs2,0) == 1) && (ctrs2 != NULL)) {
	  counters_delete(ctrs2);
	}
        return result;
      }
      i++;
    }

    // loop through words list from 'start' to 'end' indexes
    i=start;
    while ((i != end) && (strcmp(words[i],"\0") != 0)) {
      // ignore 'and' operator
      if (strcmp(words[i], "and") != 0) {
	// 'and' operands are the first word encountered and rest of
	// words since there are just 'and' operations left to do 
        ctrs1 = calculate_scores(index, words, i, i);

	if (strcmp(words[i+1], "and") == 0) {
	  i++;
	}

	ctrs2 = calculate_scores(index, words, i+1, end);
	
	// performing 'and' operation
	result = and_counters(ctrs1, ctrs2);
        
        // freeing used counters if not part of index
        if ((counters_get(ctrs1, 0) == 1) && (ctrs1 != NULL)) {
          counters_delete(ctrs1);
        }
        if ((counters_get(ctrs2,0) == 1) && (ctrs2 != NULL)) {
          counters_delete(ctrs2);
        }
	return result;
      }
      i++;
    }
  }
  // if empty, return NULL
  return NULL;
}


/*
 *
 *
 */
counters_t *or_counters(counters_t *ctrs1, counters_t *ctrs2) 
{
  // exits if NULL parameters are recieved
  assertp(ctrs1, "or_counters recieves NULL ctrs1\n");
  assertp(ctrs2, "or_counters recieves NULL ctrs2\n");

  counters_t *sumCtrs=NULL;

  if ((sumCtrs=counters_new()) != NULL) {
  
    counters_iterate(ctrs1, sumCtrs, copy_counters);
    counters_iterate(ctrs2, sumCtrs, sum_counters);
 
    counters_set(sumCtrs, 0, 1);
    
    return sumCtrs;
  }
  return NULL;
}

void sum_counters(void *ctrs, const int docId, const int count) 
{
  // adding up the two counts for a docId and set it in ctrs
  counters_set(ctrs, docId, counters_get(ctrs, docId)+count);
}

void copy_counters(void *ctrs, const int docId, const int count)
{
  counters_set(ctrs, docId, count);
}

counters_t *and_counters(counters_t *ctrs1, counters_t *ctrs2)
{
  // exits if NULL parameters are recieved
  assertp(ctrs1, "and_counters recieves NULL ctrs1\n");
  assertp(ctrs2, "and_counters recieves NULL ctrs2\n");

  counters_t *minCtrs=NULL;

  multi_counters_t *twoCtrs=count_malloc(sizeof(multi_counters_t));
  assertp(twoCtrs, "Error allocating memory for twoCtrs\n");

  minCtrs=counters_new();
  assertp(minCtrs, "Error allocating memory for minCtrs\n");

  twoCtrs->ctrs1=minCtrs;
  twoCtrs->ctrs2=ctrs2; 
  counters_iterate(ctrs1, twoCtrs, minimum_counters);
  count_free(twoCtrs);
  counters_set(minCtrs, 0, 1);
    
  return minCtrs;
}

void minimum_counters(void *arg, const int docId, const int count)
{
  multi_counters_t *twoCtrs=arg;
  int count2 = counters_get(twoCtrs->ctrs2, docId);

  if ((count != 0) && (count2 != 0)) {
    if (count2 > count) {
      counters_set(twoCtrs->ctrs1, docId, count); 
    } else {
      counters_set(twoCtrs->ctrs1, docId, count2);
    }
  }
}

void num_counters(void *arg, const int docId, const int count)
{
  int *total = arg;

  if (docId > 0) {
    (*total)++;
  }
}

void sort_counters(void *arg, const int docId, const int count) 
{
  if (docId > 0) {
  
    document_t **result=arg;
    int i;
 
    // find first empty slot
    for (i=0; result[i] != NULL; i++) {}

    // create document for docId and count
    document_t *doc=count_malloc(sizeof(document_t));
    assertp(doc, "Error allocating memory for a document_t\n");

    doc->id=docId;
    doc->score=count;

    result[i]=doc;

    while ((i > 0) && (result[i-1]->score < result[i]->score)) {
      doc=result[i];
      result[i]=result[i-1];
      result[i-1]=doc;
      i--;
    }
  }
}

/* Prints out a prompt if recieving input from an interactive user.
 * Provided by CS50 staff
*/ 
static void prompt(void)
{
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

