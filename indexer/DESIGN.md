# TSE Indexer Design Spec

## Jacob Werzinsky, CS50, February 2020

### User Interface

The indexer's only interface with the user is on the command line, which must have two arguments and the following usage:

```bash
indexer pageDirectory IndexFilename
```

For example,

```console
$ ./indexer directory file
```

### Inputs and outputs

Inputs: A directory previously crawled by the crawler and a valid file to write an index to supplied on the command-line as shown in the User Interface above. The files in the crawler directory must have the following format as specified in the requirements spec:

	-the document id starts at 1 and increments by 1 for each new page,
	-and the filename is of form pageDirectory/id,
	-and the first line of the file is the URL,
	-and the second line of the file is the depth,
	-and the rest of the file is the page content (the HTML, unchanged).
Outputs: The index is outputted to the file supplied on the command line. It contains the word and (docId, count) pairs in the following format, where docId is short for documentId:
	-one line per word, one word per line
	-each line provides the word and one or more (docID, count) pairs, in the format:
		-word docID count [docID count]…
		-where word is a string of lower-case letters,
		-where docID is a positive non-zero integer,
		-where count is a positive non-zero integer,
		-where the word and integers are separated by spaces.
Functional decompostion into modules:
We anticipate the following modules or functions:

1. *main*, which parses arguments and initializes other modules
2. *index*, which defines the index structure and its methods
3. *index_build*, which inserts data into an index from the pageDirectory
4. *index_save*, writes the data in an index to a file
5. *webpageLoad*, Loads a page from the pageDirectory to a webpage_t
6. *word*, normalizes a word i.e. makes it all lowercase

Helper modules providing data structures:

1. *hashtable*, used to store (word, counters_t) pairs as a part of the index structure
2. *counters*, used to store (docId, count) pairs as a part of the index structure

### Pseudo code for logic/algorithmic flow

1. Execute from the command line as shown in the User Interface.
2. Parse the command line, validate parameters, initialize index structure, and open indexFilename
3. while there are still unread files within the pageDirectory,
	1. Read the next file and get its html
	2. while there are still unread words in the html,
		1. Read the next word
		2. Normalize the word
		3. if has three or more characters, insert it with the docID into the index structure
	        	1. If the pair already has a count in the index, increment its count
			2. If its not, then add them with a count of 1
4. while there are still words in the index
	1. Write a word to the indexFilename
	2. while there are still (docId, count) pairs associated with that word
		1. write that (docId, count) pair on the same line as the word
5. Free memory for index structure and any other structures used, close indexFilename

### Testing Plan

Test the indexer and indextest programs through testing.sh and ensure that each output produced matches expectations. Testing.sh will have processes that will do the following:


1. Test each program with various forms of incorrect command-line arguments to see if they are properly parsed and validated.

2. Test indexer with samples that contain words that repeat a lot, have mixed capitalization, non-alphabetic characters,  and contain words with less than 3 characters.

3. Test indexer with a relatively  small amount of crawler files in pageDirectory.

4. Test indexer with a relatively large amount of crawler files in pageDirectory.

5. Test on empty crawler directory

6. Test indextest with an empty file.

7. Test indextest with the index files produced by the indexer. Make sure the output matches the input exactly using the provided indexsort.awk file.
