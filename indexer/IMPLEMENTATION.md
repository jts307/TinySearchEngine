# Implementation for indexer

## Pseudo code

The pseudo code for the indexer goes as follows: 

### Execute from the command line as shown in the User Interface.

### Parse the command line, validate parameters, initialize index structure, and open indexFilename

### while there are still unread files within the pageDirectory,

#### Read the next file and get its html

#### while there are still unread words in the html,

##### Read the next word

##### Normalize the word (make lowercase)

##### if has three or more characters, insert it with the docID into the index structure 

###### If the pair already has a count in the index, increment its count 

###### If its not, then add them with a count of 1

### while there are still words in the index

#### Write a word to the indexFilename

#### while there are still (docId, count) pairs associated with that word

##### write that (docId, count) pair on the same line as the word

### Free memory for index structure and any other structures used, close indexFilename

## Functions:

### indexer.c

```c
int main(const int argc, const char *argv[]);
```
The main function takes the arguments from the command line and makes sure that there is two of them. It also checks whether or not they are valid. For pageDirectory, it passes it to `bool isCrawlerDirectory(char* pageDirectory)` to check if it is a readable crawled directory. For IndexFilename, it attempts to open a file stream for writing using `fopen` to test if it is a writable file if the file already exists.

```c
int index_build(index_t *index, const char *pageDirectory);
```
Creates the entire index, i.e. inserts into the index based on data from the pageDirectory recieved through webpageLoad, number 4 of the pseudo encapsulates much of what this function does.

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
webpage_t *webpageLoad(const char *pageDir, int id);
```
Creates a `webpage struct` containing the html, url and depth of a file idenitified by id and returns it to the caller. Used to fetch data from the pageDirectory.

## Data Structures:

The program makes use of five data structures, three of which were created in the cs50 lab 3 assignment: `counters struct`, `set struct` and `hashtable struct`. The fourth one is the `webpage struct` provided in the lab4 assignment and the fifth was created in the lab5 assignment, `index struct`.

### Index

The `index struct` implements a mapping from words to (documentId, count) pairs, where a documentId is the name of a file in the page directory and the count is the number of times a word appears in documentId. It does not store words less then 3 characters and lowercases all words before insertion. The index is implemented through [index.c](../common/index.h) and more information on index functions can be found in [index.h](../common/index.h). The following functions involving the `index struct` are used in the indexer implementation:

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
The `index struct` was used to hold the word to (documentId, count) pairs so that they can be collected to create an index and later written to an output file.

### Counters

The `counters struct` implements a set of counters, each disinguised by an integer key. Each key can only occur once in the set and a counter keeps track of how many of the key there are. It starts empty and each time a key is inserted a count is increment by 1. The current counter for a key can be retrieved by calling the apppropriate function. Counters is implemented through [counters.c](../libcs50/counters.h). The following functions involving the `counters struct` are used in the indexer implementation:

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
The `counters struct` was used to hold documentId and count pairs within the `index struct`.

### Webpage

The `webpage struct` holds the url, html and depth of a webpage. The url and html are stored as `char*` while the depth is an `int`. The following functions involving the `webpage struct` are used in the indexer implementation:

```c
// allocates memory for a new webpage struct
webpage_t *webpage_new(char *url, const int depth, char *html);
// frees any memory used by the passed webpage struct
void webpage_delete(void *data);
// returns a word from a webpage's html
char *webpage_getNextURL(webpage_t *page, int *pos);
```

The `webpage struct` was used to parse information from the files within the pageDirectory and cycle through the html of a file word by word. More information on how these functions work can be found in [webpage.h](../libcs50/webpage.h).

### Set

The `set struct` implements a unordered collection of (key,item) pairs, where each key only occurs once in the set. A set starts empty and grows as more pairs are added to it but pairs cannot be removed or updated. The set implements a set of `void*` which are identified by their `char*` keys . The following functions are used in the indexer program:

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

The `hashtable struct` is just like a `set struct` but is more efficient in storing large collection of items. It uses an array of `set structs` to store items which allows it to retrieve items quicker. The following functions are used in the indexer program:

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
```

The `hashtable struct` was used to store the word and counters pairs in the index structure.

## Security and privacy properties:
The indexer does not interface with any outside websites or other sources, so there is nothing particular to note in this category.

## Error handling and recovery:
Indexer handles most of its errors by simply writing them to standard error through the use of fprintf statements, for example:

```c
fprintf(stderr, "pageDirectory is not a valid readable Crawler directory.\n");
status+=2;
 ```
The program will log the error and continue operating for most errors unless it is an error that interfers with the program entirely like a missing pageDirectory, in which case it exits. Also it exits if there is an issue with memory allocation making use of the [memory module's](../libcs50/memory.h) `assertp` function.

## Resource management:
The indexer utilizes heap memory by calling `malloc` and `free` through the [memory module's](../libcs50/memory.h) `count_malloc` and `count_free` which add an extra functionality of keeping a count of how many mallocs and frees took place within a program. 

## Persistant storage:
The program creates a file that contains the index mapping as described in the [README](README.md).

