/* 
 * querier.c - Answers queries from stdin based on the passed crawler directory and index file.
 *	       It prints to stdout all documents, if any, that match a given query.
 * usage: querier [pageDirectory] [indexFilename]
 * input:
 * 	- pageDirectory: pathname of a directory produced by the Crawler, must have
 *	  crawler files in proper crawler syntax.
 * 	- indexFilename: pathname of a file produced by the Indexer, must be in 
 *	  index file format.
 *	- Queries to standard input, must have the syntax described 
 *	  in the DESIGN.md for this program. 
 * output:
 * 	- Answers to queries. Each answer will print to stdout all documents that match a query
 *        ranked from highest to lowest by score. 
 * Exit statuses:
 *   0 - success
 *   1 - Usage Error
 *   2 - Invalid pageDirectory
 *   3 - non-readable IndexFilename
 *   4 - Failure to initialize index struct
 *   5 - Failure to load index file information into index struct
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

// used to store documents and their
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
                
		// finding all documents that match the query and each of their scores
		if ((scores=calculate_scores(index, cleanedInput, 0, -1)) != NULL) {
		    
		  // reseting total to 0
		  total=0;	
			
		  // counting the number of documents that matched the query
		  counters_iterate(scores, &total, num_counters); 
		    
		  if (total==0) {
		      printf("No documents match.\n");
		  } else {
		      // array of sorted documents
		      sortedScores=count_calloc(total, sizeof(document_t));
		      assertp(sortedScores, "Error allocating memory for array of document_t.\n");

		      // filling array with sorted (document, score) pairs each 
		      // pair within a document struct, sorted by score
		      counters_iterate(scores, sortedScores, sort_counters);

		      printf("Matches %d documents (ranked):\n", total);
			  
		      // looping through the sorted documents
		      for (int i=0; i<total; i++) {
    				
			// getting document id
		        int id=sortedScores[i]->id;  
			      
			// getting url for a document     
			const char *url=getPageURL(argv[1],id);
			      
		        printf("score %4d doc %4d: %s\n", sortedScores[i]->score, id, url);
		        
			// freeing memory used
			if (url != NULL) {
			  count_free((char*)url);
			}
			count_free(sortedScores[i]);
		      }
		      printf("-----------------------------------------------\n");
		      count_free(sortedScores);
		  }
	          // freeing counters if marked
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
          // if first word then its a syntax error
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
      // add marker to denote end of array
      words[count] = "\0";
	  
      return words;
  }
}

/* Recursively computes the scores of a query. Scores refering to the documents that satisfy a query
 * and their respective scores. Each (document, score) pair is stored within the returned counters struct.
 * Parameters:
 * 	-index: the index that is being looked through
 *	-words: the query that is being searched, seperated to an array of words by clean_input. words must
 *               have a '\0' character to indicate an end to the array.
 *	-start: the start of the range of the query currently being searched
 *	-end: the end of the range of the query currently being searched (indicate a -1 to continue until end of
 *	      words array)
 * returns:
 * 	-scores: a counters struct that contains (documentId, score) pairs where a documentId is the name
 *	         of a document that satifies the given query and score is the score that document recieves.
 *	-NULL: if the words list is empty
 * -On any memory allocation errors or null arguments it exits with non-zero status
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
      // if a word does not have a counters struct in the index
      // make an empty one for it
      if ((ctrs1=index_find_ctrs(index, words[start])) == NULL) {
        ctrs1=counters_new();
	assertp(ctrs1, "Error allocating memory for ctrs1\n");
	
	// mark it so that it can be freed later
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

	// freeing marked counters if needed
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
        
	// get scores for first not 'and' word encountered
	ctrs1 = calculate_scores(index, words, i, i);

	// incrementing over 'and' operator in words array
	if (strcmp(words[i+1], "and") == 0) {
	  i++;
	}
	// calculating scores for rest of words
	ctrs2 = calculate_scores(index, words, i+1, end);
	
	// performing 'and' operation
	result = and_counters(ctrs1, ctrs2);
        
        // freeing marked counters if needed
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
  // if empty array, return NULL
  return NULL;
}

/* Performs an 'or' operation on two counters struct. The 'or' operation sums up all the counters of both
 * counters struct into one counters struct. This does not edit the two counters struct involved in the 
 * operation.
 * Parameters:
 * 	-ctrs1: first counters struct to be combined
 *	-ctrs2: second counters struct to be combined
 * returns:
 * 	-sumCtrs: a combination of ctrs1 and ctrs2 that sums all their counters.
 * -On null arguments it exits with non-zero status
 */
counters_t *or_counters(counters_t *ctrs1, counters_t *ctrs2) 
{
  // exits if NULL parameters are recieved
  assertp(ctrs1, "or_counters recieves NULL ctrs1\n");
  assertp(ctrs2, "or_counters recieves NULL ctrs2\n");

  // combined counters struct
  counters_t *sumCtrs=NULL;

  if ((sumCtrs=counters_new()) != NULL) {
    // copying the counters of ctrs1 into sumCtrs
    counters_iterate(ctrs1, sumCtrs, copy_counters);
	 
    // adding the counters of ctrs2 to sumCtrs
    counters_iterate(ctrs2, sumCtrs, sum_counters);
 
    // marking sumCtrs for later deletion
    counters_set(sumCtrs, 0, 1);
    
    return sumCtrs;
  }
  return NULL;
}

/* Used to combine two counters structs. Used as a parameter to counters_iterate.
 * Parameters:
 * 	-ctrs: counters struct that will be added to the counters_struct 
 *             currently being iterated over. The results of the summation
 *	       will be saved here.
 * 	-docId: current docId being iterated over and added to ctrs.
 *	-count: count of the current docId, added to ctrs.
 * returns:
 * 	-nothing
 */
void sum_counters(void *ctrs, const int docId, const int count) 
{
  // adding up the two counts for a docId and setting it in ctrs(sumCtrs)
  counters_set(ctrs, docId, counters_get(ctrs, docId)+count);
}

/* Used to copy one counters struct to another. Used as a parameter to counters_iterate.
 * Parameters:
 * 	-ctrs: counters struct that will have information copied to it.
 * 	-docId: current docId being iterated over and copied to ctrs.
 *	-count: count of the current docId, copied to ctrs.
 * returns:
 * 	-nothing
 */
void copy_counters(void *ctrs, const int docId, const int count)
{
  // copying the iterated counters struct values to ctrs
  counters_set(ctrs, docId, count);
}

/* Performs an 'and' operation on two counters struct. The 'and' operation finds the minimum count of each 
 * documentId within both structs and sets that as the count for the documentId in a third counters struct. 
 * This does not edit the two counters passed as arguments.
 * Parameters:
 * 	-ctrs1: first counters struct to be minimized
 *	-ctrs2: second counters struct to be minimized
 * returns:
 * 	-minCtrs: the minimization of ctrs1 and ctrs2.
 * -On null arguments and errors involving memory allocatoin it exits with non-zero status
 */
counters_t *and_counters(counters_t *ctrs1, counters_t *ctrs2)
{
  // exits if NULL parameters are recieved
  assertp(ctrs1, "and_counters recieves NULL ctrs1\n");
  assertp(ctrs2, "and_counters recieves NULL ctrs2\n");

  counters_t *minCtrs=NULL;
	
  // initializing multi_counters struct to store two counters
  multi_counters_t *twoCtrs=count_malloc(sizeof(multi_counters_t));
  assertp(twoCtrs, "Error allocating memory for twoCtrs\n");
  
  // allocating memory for minCtrs
  minCtrs=counters_new();
  assertp(minCtrs, "Error allocating memory for minCtrs\n");

  // bundling minCtrs and ctrs2 into one struct
  twoCtrs->ctrs1=minCtrs;
  twoCtrs->ctrs2=ctrs2; 
	
  // iterating over ctrs1 with the new struct to get
  // the minimum of ctrs1 and ctrs2
  counters_iterate(ctrs1, twoCtrs, minimum_counters);
	
  count_free(twoCtrs);
	
  // marking minCtrs for deletion later on 
  counters_set(minCtrs, 0, 1);
    
  return minCtrs;
}

/* Used to find the minimum of two counters structs. Used as a parameter to counters_iterate.
 * Parameters:
 * 	-arg: multi_counters struct that contains the counters struct that the minimums
 	      will be saved to in ctrs1 and one of the counters struct to be minimized in ctrs2.
 * 	-docId: current docId being iterated over.
 *	-count: count of the current docId.
 * returns:
 * 	-nothing
 */
void minimum_counters(void *arg, const int docId, const int count)
{
  // casting void* to multi_counters_t*
  multi_counters_t *twoCtrs=arg;

  // getting the current docId's count within the other counters struct
  int count2 = counters_get(twoCtrs->ctrs2, docId);

  // do not set docId in minCtrs if its count is 0
  if ((count != 0) && (count2 != 0)) {
    // set docId in minCtrs with the lower of the two counts
    if (count2 > count) {
      counters_set(twoCtrs->ctrs1, docId, count); 
    } else {
      counters_set(twoCtrs->ctrs1, docId, count2);
    }
  }
}

/* Used to find the number of documents Ids(keys) within a counters struct, ignores key 0. 
 * Used as a parameter to counters_iterate.
 * Parameters:
 * 	-arg: a pointer to the int that the total will be saved to.
 * 	-docId: current docId being iterated over.
 *	-count: count of the current docId.
 * returns:
 * 	-nothing
 */
void num_counters(void *arg, const int docId, const int count)
{
  // casting void* to int*
  int *total = arg;

  // this is to avoid the 0 docId set on 
  // counters that need to be freed
  if (docId > 0) {
    (*total)++;
  }
}

/* Stores the (docId, count) pairs into document structs, then sorts these document structs
 * by count from highest to lowest within an array using the insertion sort algorithm. 
 * Used as a parameter to counters_iterate.
 * Parameters:
 * 	-arg: an array of document structs, initialized with enough space to hold all (docId, count)
 *.           pairs within the counters struct being iterated over. 
 * 	-docId: current docId being iterated over.
 *	-count: count of the current docId.
 * returns:
 * 	-nothing
 */
void sort_counters(void *arg, const int docId, const int count) 
{
  // do not add the 0 docId counter, only used as marker
  // for memory allocation
  if (docId > 0) {
  
    // casting void* to document_t**
    document_t **result=arg;
    int i;
 
    // find first empty slot
    for (i=0; result[i] != NULL; i++) {}

    // create document for docId and count
    document_t *doc=count_malloc(sizeof(document_t));
    assertp(doc, "Error allocating memory for a document_t\n");
	  
    // set the docId and count in document struct
    doc->id=docId;
    doc->score=count;

    // store document in empty slot
    result[i]=doc;

    // loop backwards through the array
    while ((i > 0) && (result[i-1]->score < result[i]->score)) {
      // if two values are out of place, swap them
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
