# Implementation for indexer

## Pseudo code

The pseudo code for the indexer goes as follows: 

### Execute from the command line as shown in the User Interface.

Parameters are passed to `int main(const int argc, const char *argv[])` which then checks that there are exactly 3 arguments passed from the command line which include `const char *pageDirectory` and `const char *indexFilename`.

### Parse the command line, validate parameters, initialize index structure, and open indexFilename

The main function then validates its parameters by making calls to `IsCrawlerDirectory` within the *pagedir* module on `const char *pageDirectory` to check if the directory is a valid crawler directory, and `fopen` with "w" option on `const char *indexFilename` to check if `indexFilename` is a writable file if it exists. It then calls `index_t *index_new(const int num_slots)` to initialize/allocate memory for an index structure.

### while there are still unread files within the pageDirectory,
#### Read the next file and get its html

Main then calls index_build, which then goes in a while loop with `webpage_t *webpageLoad(const char *pageDir, int id)` passing the `pageDirectory` each loop and an `int` starting at 1. This `int` increments every loop and `webpageLoad` returns a `webpage_t` that has the html, url and depth of the file with documentId `int` in `pageDirectory`. This loop continues until there is no file with the `int` documentId, in which case the `webpageLoad` returns NULL and the loop terminates.

#### while there are still unread words in the html,
##### Read the next word

`char *webpage_getNextWord(webpage_t *page, int *pos)` is called in a while loop passing the `webpage_t` from the previous step and an `int` counter that is used by the the function to keep track of which word it is on. The function then returns the next `char *` word if there is one, in the case where there is not it returns NULL and the loop terminates.

##### Normalize the word (make lowercase)

The returned word is then normalized using `int normalizeWord(char *word)` which takes the word and lowercases any alphabetic characters within it.

##### if has three or more characters, insert it with the docID into the index structure 

The length of the word is checked with `strlen()` to see if it has three or more chars. If it does then the word and the current `int` documentId is passed to `bool index_insert(index_t *index, const char *word, const int docId, const int wordCount)`. 

###### If the pair already has a count in the index, increment its count 
###### If its not, then add them with a count of 1
`const int wordCount` is calculated by taking the current wordCount of the (word, documentId) pair in the `index struct` using the function `int index_find(index_t *index, const char *word, const int docId)` and then adding 1. This is then passed to `index_insert` which then sets the (word, documentId, wordCount) trio in the `index struct`.

### while there are still words in the index

Control is then returned to `main` which then calls `void index_save(index_t *index, FILE *fp)`. Within `index_save`, `void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item) )` is called on the `index struct`'s `hashtable struct`. The function goes through each (word, (documentId, wordCount)) pair similar to a while loop.

#### Write a word to the indexFilename

 A local function in place of `itemfunc` and `indexFilename`(as a file stream) in place of `void *arg` are passed to `hashtable_iterate`. The local function writes each word to `indexFilename` using a `fprintf` statement.

#### while there are still (docId, count) pairs associated with that word

This function then calls `void counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg, const int key, const int count))` which goes through each (documentId, wordCount) pair contained in `counters` associated with each word within the `index struct`, again similar to a while loop, and `IndexFilename` and a different local function are passed to `counters_iterate` as well.

##### write that (docId, count) pair on the same line as the word

Each (documentId, wordCount) pair is then written to `IndexFilename` through this other local function passed to `counters_iterate`, using a `fprintf` statement.

### Free memory for index structure and any other structures used, close indexFilename

Lastly, control is returned to `main` and `indexFilename` is closed using `fclose` and the `index struct` is then freed from memory using `void index_delete(index_t *index)`.

## Functions:

### querier.c

```c
int main(const int argc, const char *argv[]);
```
The main function takes the arguments from the command line and makes sure that there is two of them. It also checks whether or not they are valid. For pageDirectory, it passes it to `bool isCrawlerDirectory(char* pageDirectory)` to check if it is a readable crawled directory. For IndexFilename, it attempts to open a file stream for reading using `fopen` to test if it is a readable file. Then, it allocates space for and initializes the index structure by calling `index_new()`, and fills this index with information from IndexFilename by calling `index_load()`. After this, it reads from the standard input using the `readlinep()` function from file.h. It 

```c
char **clean_input(char *input);
```

```c
counters_t *calculate_scores(index_t *index, char **words, int start, int end); 
```

```c
counters_t *or_counters(counters_t *ctrs1, counters_t *ctrs2); 
```

```c
void sum_counters(void *ctrs, const int docId, const int count); 
```

```c
void copy_counters(void *ctrs, const int docId, const int count);
```

```c
counters_t *and_counters(counters_t *ctrs1, counters_t *ctrs2);
```

```c
void minimum_counters(void *arg, const int docId, const int count);
```

```c
void num_counters(void *arg, const int docId, const int count);
```

```c
void sort_counters(void *arg, const int docId, const int count);
```



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

