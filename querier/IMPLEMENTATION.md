# Implementation for querier

## Pseudo code

The pseudo code for the querier goes as follows: 

### Execute from the command line as shown in the User Interface.
### Parse the command line, validate parameters, initialize index structure, and open indexFilename
### while there are still lines a file
#### read the first word of each line
#### while there are still docIds and word counts that comes after that word
##### read a docId and word count pair 
##### insert the (word, docId, word count) into the index structure
##### close indexFilename
### while there is still input to be read from stdin 
#### read a line from the stdin, a query
#### Make sure the query follows proper query syntax
#### lower case all letters within the query and replace all series of spaces with one space
#### Print this version of the query to standard output
#### Call a function calculate_scores on the query
##### search for the first occurance of an 'or' in the query, if any.
###### call calculate_scores on the parts of the query seperated this 'or'.
###### perform an 'or' operation on the scores returned for parts of the query. 
##### if there is more than one word in the sequence
###### call calculate_scores on the first word, other than 'and', encountered.
###### call calculate_scores on the rest of the words in the query.
###### perform an 'and' operation on the scores returned from 1 and 2.
##### if there is only one word in the query then find its scores by searching the index, getting the (docId, score) pairs associated with the word, where score=count (Base Case).
#### Iterate through each (docId, score) pair obtained from calculate_scores and count how many there are.
#### Use the insertion sort algorithm to sort the scores obtained from calculate_scores.
#### while there are still (docId, score) pairs
##### open the document in the pageDirectory identified by a docId.
##### read the first line of this document, this is the url of this document.
#### Print the number of matches for the query, i.e. the number of (docId, score) pairs. Print the document Ids and their scores ranked from highest to lowest. Along with the document Ids print the url obtained from step 9.
### Free memory for the index structure and any other structures used

## Functions:

### querier.c

```c
int main(const int argc, const char *argv[]);
```
The main function takes the arguments from the command line and makes sure that there is two of them. It also checks whether or not they are valid. For pageDirectory, it passes it to `bool isCrawlerDirectory(char* pageDirectory)` to check if it is a readable crawled directory. For IndexFilename, it attempts to open a file stream for reading using `fopen` to test if it is a readable file. Then, it allocates space for and initializes the index structure by calling `index_new()`, and fills this index with information from IndexFilename by calling `index_load()`. After this, it reads a line from the standard input using the `readlinep()` function from [file.h](../libcs50/file.h) and passes this to several other functions for interpretation. It then prints using `printf` to the standard output the scores calculated from these interpretations. Lastly, it frees memory for any data structures used, including `counters struct`s used and the `index struct` used.  

```c
char **clean_input(char *input);
```
The function 'cleans' a `char*` by seperating it into an array of `char*` based on space delimiters (space delimiters including tabs, multiple spaces, etc.). It also makes sure that the `char*` follows proper query syntax: there are only spaces and alphabetic characters, 'and' and 'or' operators cannot appear next to one another and cannot appear at the beginning or end of a query. It adds a '\0' character
at the end of the array to mark the end of the array, this makes looping over the array easier.

```c
counters_t *calculate_scores(index_t *index, char **words, int start, int end); 
```
This function recursively computes the scores for a query that was 'cleaned' by `clean_input`. The scores are stored in a `counters struct` in (documentId, score) pairs. The scores are obtained first directly from the index previously initialized, then through the 'or's and 'and's of these scores. To do 'or' and 'and' operations on two `counters structs` this function calls `or_counters` and `and_counters`

```c
counters_t *or_counters(counters_t *ctrs1, counters_t *ctrs2); 
```
The function takes two `counters struct`s and copies one to a third `counters struct`, call this *sum*, by passing `copy_counters` and *sum* to `counters_iterate` which iterates over one of the two `counters struct`. It then iterates over the other of the two `counters struct`s with *sum* and `sum_counters` as parameters, which adds all the counters in `counter struct` being iterated over to *sum*. *sum* is then returned to the caller.

```c
void sum_counters(void *ctrs, const int docId, const int count); 
```
This function is used as a parameter for `counters_iterate`. As it is iterating over a `counters struct` it adds the `count` for a `docId` to the count of the same `docId` in `ctrs`, a different `counters struct`. This sum is then set in `ctrs` for the `docId` being iterated over. 

```c
void copy_counters(void *ctrs, const int docId, const int count);
```
This function is used as a parameter for `counters_iterate`. As it is iterating over a `counters struct`, it copies each of its `docId`s and `count`s to another `counters struct`, i.e. `ctrs`.

```c
counters_t *and_counters(counters_t *ctrs1, counters_t *ctrs2);
```
This function takes two `counters struct`s and combines them into a single `counters struct`, called `minCtrs`, that contains the minimum counts for each document id contained within both, except for the cases where 0 is the minimum value then the document id is excluded all together. It does this by bundling `minCtrs` and one of the two `counters struct`s into a `multi_counters struct`. The multi-counters is then passed to `counters_iterate` with the function `minimum_counters` which calculates the minimum of the two `counters struct`s and stores that in `minCtrs`.

```c
void minimum_counters(void *arg, const int docId, const int count);
```
This function is used as a parameter for `counters_iterate`. As it is iterating over a `counters struct`, it gets the count for `docId` within the other `counters struct` stored within `multi_counters`(which is passed as `void *arg`). It then determines if `count` or the count gotten from the other `counters struct` is smaller, which ever one is is set within `minCtrs` using `counters_set()`.

```c
void num_counters(void *arg, const int docId, const int count);
```
This function is used as a parameter for `counters_iterate`. The `void *arg` is a `int *` which will store the number of documentId's with non-zero counts. As it is iterating over a `counters struct`, it increments the `int` pointed to by `int*`
by 1 if a `docId` with a `count` greater than 0 is encountered.

```c
void sort_counters(void *arg, const int docId, const int count);
```
This function is used as a parameter for `counters_iterate`. The `void *arg` is a `document_t **`. The function takes each (`docId`, `count`) pair being iterated over, creates a `document struct` for each pair, and stores a `document *` to each `document struct` within the array of `document *`. The `document *`s are sorted in decreasing order using the insertion sort algorithm as they are inserted into the array.

### word.c

```c
int normalizeWord(char *word);
```
Normalizes a word by lowercasing all its alphabetic characters. Used in *index* module.

### pagedir.c

```c
bool isCrawlerDirectory(const char *pageDir);
```
The isCrawlerDirectory function takes a passed directory and checks whether it is an existing, readable and crawler visited directory by attempting to read a .crawler file in it left behind by `crawler.c`.

```c
const char *getPageURL(const char *pageDir, int id);
```
The getPageURL function takes a passed directory and document id, opens the file within the passed directory with the name document id, reads the first line of the document using `freadlinep`
from the *file.h* module, and returns this to the caller, this is the url of the document.

## Data Structures:

The program makes use of six data structures, three of which were created in the cs50 lab 3 assignment: 
`counters struct`, `set struct` and `hashtable struct`. The fourth one was created in the lab5 assignment, 
`index struct`. The last two `multiCounters struct` and `document struct` were created in the `querier.c` 
program, local to it.

### Index

The `index struct` implements a mapping from words to (documentId, count) pairs, where a documentId is the name of a file in the page directory and the count is the number of times a word appears in documentId. It does not store words less then 3 characters and lowercases all words before insertion. The index is implemented through [index.c](../common/index.h) and more information on index functions can be found in [index.h](../common/index.h). The following functions involving the `index struct` are used in the querier implementation:

```c
// allocating memory for index structure
index_t *index_new(const int num_slots);
// setting a word to (document id,count) pair in the index structure 
bool index_insert(index_t *index, const char *word, const int docId, const int wordCount);
// gets the count for a (word, document id) pair
int index_find(index_t *index, const char *word, const int docId);
// prints out the index structure in a human friendly format
void index_print(index_t *index, FILE *fp);
// frees memory taken up by index structure
void index_delete(index_t *index);
// writes the contents of the index to a file
void index_save(index_t *index, FILE *fp);
// takes the data from an index file and loads it into an index
int index_load(index_t *index, FILE *fp);
```
The `index struct` was used to hold the word to (documentId, count) pairs so that they can be collected to create an index.

### Counters

The `counters struct` implements a set of counters, each disinguised by an integer key. Each key can only occur once in the set and a counter keeps track of how many of the key there are. It starts empty and each time a key is inserted a count is increment by 1. The current counter for a key can be retrieved by calling the apppropriate function. Counters is implemented through [counters.c](../libcs50/counters.h). The following functions involving the `counters struct` are used in the querier implementation:

```c
// allocates memory for a new counters struct
counters_t *counters_new(void);  
// gets the count for a key
int counters_get(counters_t *ctrs, const int key);
// sets the count for a key
bool counters_set(counters_t *ctrs, const int key, const int count);
// prints the counters struct and its contents
void counters_print(counters_t *ctrs, FILE *fp);
// performs an operation on each (key, count) pair
void counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg, const int key, const int count));
// frees memory taken up by counters struct
void counters_delete(counters_t *ctrs);
```
The `counters struct` was used to hold documentId and count pairs within the `index struct`. They were also used throughout
the querier program to store (documentId, score) pairs.

### Set

The `set struct` implements a unordered collection of (key,item) pairs, where each key only occurs once in the set. A set starts empty and grows as more pairs are added to it but pairs cannot be removed or updated. The set implements a set of `void*` which are identified by their `char*` keys . The following functions are used in the querier program:

```c
// allocated memory for a new set struct
set_t *set_new(void);
// inserts a key and item pair into the set
bool set_insert(set_t *set, const char *key, void *item);
// finds a key and item pair within the set
void *set_find(set_t *set, const char *key);
// prints what is contained within a set
void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item) );
// frees memory taken up by the set
void set_delete(set_t *set, void (*itemdelete)(void *item) );
```

The `set struct` was utilized by the `hashtable struct` to hold word and counters pairs. None of its functions were directly called by crawler but instead indirectly used through the `hashtable struct`. For more information on how these functions work can be found in [set.h](../libcs50/set.h).

### Hashtable

The `hashtable struct` is just like a `set struct` but is more efficient in storing large collection of items. It uses an array of `set structs` to store items which allows it to retrieve items quicker. The following functions are used in the querier program:

```c
// allocates memory for a hashtable struct
hashtable_t *hashtable_new(const int num_slots);
// inserts a new key and item pair into the hashtable
bool hashtable_insert(hashtable_t *ht, const char *key, void *item);
// finds a key and item pair within the hashtable
void *hashtable_find(hashtable_t *ht, const char *key);
// prints what is contained within a hashtable
void hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item));
// frees memory taken up by the hashtable
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) );
// iterates through each key and item pair in the hashtable executing the passed function on them
void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item) );
```

The `hashtable struct` was used to store the word and counters pairs in the index structure.

### multiCounters

The `multiCounters struct` is two `counters struct`. This struct has no methods. It was used to iterate over
a `counters struct` with three `counters struct`s within scope. This made the process of creating a `minCtrs`
for `and_counters()` easier. 

### document

The `document struct` holds two `int`s, one for a document's id and another for its score for a given query.
This struct has no methods. It was used to act as a medium to store document ids and scores so that they can 
be sorted since `counters struct`s do not support sorting.

## Security and privacy properties:
The querier does not interface with any outside websites or other sources, so there is nothing particular to note in this category.

## Error handling and recovery:
Querier handles most of its errors by simply writing them to standard error through the use of fprintf statements, for example:

```c
 fprintf(stderr, "indexFilename must be readable.\n");
	status+=3;
 ```
The program will log the error and continue operating for most errors unless it is an error that interfers with the program entirely like a missing pageDirectory, in which case it exits. Also it exits if there is an issue with memory allocation making use of the [memory module's](../libcs50/memory.h) `assertp` function.

## Resource management:
The querier utilizes heap memory by calling `malloc` and `free` through the [memory module's](../libcs50/memory.h)  
`count_malloc` and `count_free` which add an extra functionality of keeping a count of how many mallocs and frees took 
place within a program. 

## Persistant storage:
The program creates no persistant files by default.

