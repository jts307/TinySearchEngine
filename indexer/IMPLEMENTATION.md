# Implementation for indexer

## Pseudo code

The pseudo code for the indexer goes as follows: 


### Execute from a command line as shown in the User Interface:

Parameters are passed to `int main(const int argc, const char *argv[])` which then checks that there are exactly 4 arguments passed from the command line which are `const char *seedURL`, `const char *pageDirectory` and `const char maxDepth`.

### Parse the command line, validate parameters, initialize other modules

The main function then validates its parameters by making calls to `IsValidDirectory` within the *pagedir* module on `const char *pageDirectory` to check if the directory is valid, `IsInternalURL` to check if the `const char *seedURL` is valid and internal, and checks directly that `const char maxDepth` is both numerical and nonnegative transforming it into `int maxDepth` in the process.

### make a webpage for the seedURL, marked with depth=0

The main function then passes the three arguments (`int maxDepth` not the `const char maxDepth`) to `int crawler(const char *seedURL, const char *pageDirectory, const int maxDepth)`. The crawler function then makes a `webpage struct` for the `const char seedURL` and passes it a NULL html and a depth of 0 using `webpage_new`.

### add that page to the bag of webpages to crawl

A pointer to the `webpage struct` created for the seedURL is then added to a `bag struct` of webpage pointers to crawl. 

### add that URL to the hashtable of URLs seen

The `const char seedURL` is then copied and that copy is placed within a `hashtable struct` of `const char ` urls which are the keys and empty strings which act as a placeholder for an item.

### while there are more webpages to crawl,
### extract a webpage (URL,depth) item from the bag of webpages to be crawled,

Then while there are still webpages availible to extract from the bag of webpages using `bag_extract`, a webpage is extracted. This is done with a while statement with `bag_extract` as the conditional.

### pause for at least one second,
### use pagefetcher to retrieve a webpage for that URL,

The extracted webpage is then passed to `webpage_fetcher` which gets the html for a webpage and replaces in place of the webpage's NULL html.

### use pageSaver to write the webpage to the pageDirectory with a unique document ID

The extracted webpage is then passed to `pageSaver` within the *pagedir* module which then writes the webpage's html, url and depth to a file with a numbered id as the file's name.

### if the webpage depth is < maxDepth, explore the webpage to find links:

After `pageSaver` returns then the webpage's depth is checked with `webpage_getDepth` and if it is below `int maxDepth`. 

### use pagescanner to parse the webpage to extract all its embedded URLs;

If it is below it then the function goes it a while loop where as long there are more URLs on the html of the webpage then then loop continues. This is done through `webpage_getNextURL`.

### for each extracted URL,

#### ‘normalize’ the URL
#### if that URL is not ‘internal’, ignore it;

The extracted url is then normalized and checked if it is internal at the same time through the funciton `IsInternalURL`.

#### try to insert that URL into the hashtable of URLs seen

Then it is an attempt to insert the URL into the hashtable is made through `hashtable_insert`.

#### if it was already in the table, do nothing;

If `hashtable_insert` returns true then nothing happens.

#### if it was added to the table,

If it returns false then the following happens.

#### make a new webpage for that URL, at depth+1

The extracted url is first copied to `cpyNextURL`. Then this copied url is passed to `webpage_new` along with the depth of the webpage that was extracted from the bag plus 1 using the `webpage_getDepth` method. It is also given NULL html. This creates a new webpage.

#### add the new webpage to the bag of webpages to be crawled

Then this new webpage is passed to `bag_insert` to be inserted into the bag of webpages to be visited. The loop continues until all webpages within `int maxDepth` are reached.

## Functions:

### indexer.c







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

