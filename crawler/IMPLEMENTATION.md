# Implementation for crawler

## Pseudo code

The pseudo code for the crawler goes as follows: 


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

### Crawler.c

```c
int main(const int argc, const char *argv[])
```
The main function takes the arguments from the command and makes sure that there is three of them. It also 
checks whether or not they are valid. For depth, it checks that the depth is nonnegative and numerical (a number must be the first thing that appears in the argument else it will not be recognized as such). For the directory, it checks it using pagedir module's `isValidDirectory`. For the seedURL, it uses webpage module's `IsInternalURL`. It then passes these arguments off to the crawler function.

```c
int crawler(const char *seedURL, const char *pageDirectory, const int maxDepth)
```

The crawler runs the algorithm as described above, starting from make a new webpage for the seedURL. It goes to each webpage starting at the specificed seedURL. 

### pagedir.c

```c
bool isValidDirectory(const char *pageDir)
```
The isValidDirectory function takes a passed directory and checks whether it is an existing and writable directory by attempting to write a .crawler file to it using `fopen`. If it succeeds then the .crawler file is left there. 

```c
int pageSaver(const char *pageDir, webpage_t *wp)
```
The pageSaver function takes a directory and a webpage type, and then uses the webpage type's getter functions like `int webpage_getDepth(const webpage_t *page)` to obtain information about a webpage and then uses `fprintf`and `fopen` to write that information to numbered output files. The numbering of the output files starts at the first availible number counting from zero. If a number is taken by another file in the directory then the numbering of the file increments until an availible number is found.

## Data Structures:

The program makes use of four data structures, three of which were created in the cs50 lab 3 assignment: `bag struct`, `set struct` and `hashtable struct`. The fourth one is the `webpage struct` provided in the lab4 assignment.

### Bag

The `bag struct` implements a bag which is an unordered collection of items. It starts empty and items are inserted into the 
bag one by one. Extraction from the bag is free to remove any item from the bag and return it. The bag is implemented through 
[bag.c](../libcs50/bag.h) which utilizes a linked list of `void*`. More information on bag functions can be found in 
[bag.h](../libcs50/bag.h). The following functions involving the `bag struct` are used in the crawler implementation:

```c
// allocates memory for a new bag struct
bag_t *bag_new(void);  
// inserts a pointer to an item into the bag 
void bag_insert(bag_t *bag, void *item);
// extracts a pointers to an item from the bag
void *bag_extract(bag_t *bag);
// prints out the contents of a bag
void bag_print(bag_t *bag, FILE *fp, void (*itemprint)(FILE *fp, void *item));
// frees up memory taken up by a bag and its contents
void bag_delete(bag_t *bag, void (*itemdelete)(void *item) );
```
The `bag struct` was used to hold `webpage_t *`. Each `webpage_t *` contained information about a webpage that has yet to be explored specificly its url and depth.

### Webpage

The `webpage struct` hold the url, html and depth of a webpage. The url and html are stored as `char*` while the depth is an `int`. The following functions involving the `webpage struct` are used in the crawler implementation:

```c
// allocates memory for a new webpage struct
webpage_t *webpage_new(char *url, const int depth, char *html);
// frees any memory used by the passed webpage struct
void webpage_delete(void *data);
// gets the html for a webpage through a webpage's url
bool webpage_fetch(webpage_t *page);
// searches through a webpage's html for urls
char *webpage_getNextURL(webpage_t *page, int *pos);
// normalizes a url and checks if it is in http://old-www.cs.dartmouth.edu/
bool IsInternalURL(char *url);
// getter functions used to obtain information from the `webpage struct`
int   webpage_getDepth(const webpage_t *page);
char *webpage_getURL(const webpage_t *page);
char *webpage_getHTML(const webpage_t *page);
```

The `webpage struct` was used to store and get information about a webpage. More information on how these functions work can be found in [webpage.h](../libcs50/webpage.h).

### Set

The `set struct` implements a unordered collection of (key,item) pairs, where each key only occurs once in the set. A set starts empty and grows as more pairs are added to it but pairs cannot be removed or updated. The set implements a set of `void*` which are identified by their `char*` keys . The following functions are used in the crawler program:

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

The `set struct` was utilized by the `hashtable struct` to hold the URLs of webpages. None of its functions were directly called by crawler but instead indirectly used through the `hashtable struct`. For more information on how these functions work can be found in [set.h](../libcs50/set.h).

### Hashtable

The `hashtable struct` is just like a `set struct` but is more efficient in storing large collection of items. It uses an array of `set structs` to store items which allows it to retrieve items quicker. The following functions are used in the crawler program:

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

The `hashtable struct` was used to store the urls of webpages that have already been visited by the crawler. 

## Security and privacy properties:

The `webpage_fetch` method implements a 1-second delay after every fetch attempt to ensure that it 
would not trigger any secrity concerns with the websites visited. Also the crawler only crawls 
webpages within http://old-www.cs.dartmouth.edu/ which is ensured through the `IsInternalURL`
function within the *webpage* module. 

## Error handling and recovery:
Crawler handles most of its errors by simply writing them to standard error through the use of fprintf statements, for example:

```c
fprintf(stderr, "pageDirectory is not a valid and writable directory.\n");
status+=4;
 ```
The program will log the error and continue operating for most errors unless it is an error that interfers with the program entirely like a missing pageDirectory, in which it exits. Also it exits if there is an issue with memory allocation making use of the [memory module's](../libcs50/memory.h) `assertp` function.

## Resource management:
Crawler utilizes heap memory by calling `malloc` and `free` through the [memory module's](../libcs50/memory.h) `count_malloc` and `count_free` which add an extra functionality of keeping a count of how many mallocs and frees took place within a program. 

## Persistant storage:
Crawler creates a .crawler file in the directory passed by the caller. This was used to test whether or not a directory exists and is writable. The program also creates numbered files each with a unique numbered id within the passed directory, each file contains the url, html and depth of a webpage. 

