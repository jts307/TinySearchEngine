# TSE Querier Design Spec

## Jacob Werzinsky, CS50, February 2020

### User Interface

The querier has the following command line usage:

```bash
querier pageDirectory IndexFilename
```

For example,

```console
$ ./querier directory file
```

The querier also interactively answers queries entered in by the user from standard input. 
These queries are a series of words seperate by spaces, 'and's and 'or's. All queries must be 
comprised of only letters and spaces. 'and's and 'or's cannot appear next to each other in a 
query, at the beginning of a query, or at the end. 

Here are a few examples of valid imput:

```console
hello world
hello and world
hello or world
hello or world or hello world or hello and world
```

### Inputs and outputs

Inputs: A directory previously crawled by the crawler and a valid file containing the index for this crawler directory. These are supplied on the command-line as shown in the User Interface above. The files in the crawler directory must have the following format as specified in the requirements spec:

	-the document ids start at 1 and increments by 1 for each new page,
	-and the filename is of form pageDirectory/id,
	-and the first line of the file is the URL,
	-and the second line of the file is the depth,
	-and the rest of the file is the page content (the HTML, unchanged).

The index file must contain the word and (docId, count) pairs for the files in the crawler 
directory in the following format, where docId is short for documentId:
	
	-one line per word, one word per line
	-each line provides the word and one or more (docID, count) pairs, in the format:
		-word docID count [docID count]…
		-where word is a string of lower-case letters,
		-where docID is a positive non-zero integer,
		-where count is a positive non-zero integer,
		-where the word and integers are separated by spaces.

The other form of input are the queries supplied by standard input. These queries must be in 
the form as specified in the User Interface. 

Outputs: The only output are the answers to the queries made by the standard input. These answers will 
be printed out to the standard output. Each answer will print all documents that match the 
given query or print 'No documents match' if no documents match a query. For each document, its 
document id, score and the url contained within it are printed. Documents are listed based on 
their score with higher scoring documents printing first. Scores for individual word queries 
for a document are the number of times the single word query appears in that document. Single 
word queries can be connected by either spaces or 'and's which both give the minimum score for 
a series of words connected by these operators. These 'and' sequences and single word queries 
can be connected by 'or's which gives the total score of each element connected by the 
operator. For a document to match a query it will need to have a score that is greater than 0.
		
### Functional decompostion into modules:
We anticipate the following modules or functions:

1. *main*, which parses arguments, initializes other modules, reads standard input, prints to
   the standard output.
2. *index*, which defines the index structure and its methods.
3. *index_load*, which inserts data into an index file into an index structure.
4. *prompt*, prints a prompt if stdin is an interactive user, like the command prompt.
5. *clean_input*, which takes a query, makes sure a query follows proper syntax and puts into
   a standardized form that makes interpretation by other functions easier. 
6. *calculate_scores*, finds and calculates the score for all documents that satisfy the query. 
7. *getPageURL*, which returns the url contained in a numbered file within the page directory.
8. *or_counters*, which performs an 'or' operation on two sets of scores.
9.*and_counters*, which performs an 'and' operation on two sets of scores.
10.*sort_counters*, which ranks the documents from highest score to lowest.

Helper modules providing data structures:

1. *hashtable*, used to store (word, counters_t) pairs as a part of the index structure
2. *counters*, used to store (docId, count) pairs as a part of the index structure and the
   calculation of scores.
3. *set*, used by hashtable.

### Pseudo code for logic/algorithmic flow

1. Execute from the command line as shown in the User Interface.
2. Parse the command line, validate parameters, initialize index structure, and open indexFilename
3. while there are still lines a file
	1. read the first word of each line
	2. while there are still docIds and word counts that comes after that word
		1. read a docId and word count pair 
		2. insert the (word, docId, word count) into the index structure
	3. close indexFilename
4. while there is still input to be read from stdin 
	1. read a line from the stdin, a query
	2. Make sure the query follows proper query syntax
	3. lower case all letters within the query and replace all series of spaces with one
	   space
	4. Print this version of the query to standard output
	5. Call a function calculate_scores on the query
		1. search for the first occurance of an 'or' in the query, if any.
			1. call calculate_scores on the parts of the query seperated this 'or'.
			2. perform an 'or' operation on the scores returned for parts of the query. 
		2. if there is more than one word in the sequence
			1. call calculate_scores on the first word, other than 'and', encountered.
			2. call calculate_scores on the rest of the words in the query.
			3. perform an 'and' operation on the scores returned from 1 and 2.
		3. if there is only one word in the query then find its scores by searching the index,
		   getting the (docId, score) pairs associated with the word, where score=count (Base Case).
	6. Iterate through each (docId, score) pair obtained from calculate_scores and count how many there are.
	7. Use the insertion sort algorithm to sort the scores obtained from calculate_scores.
	9. while there are still (docId, score) pairs
		1. open the document in the pageDirectory identified by a docId.
		2. read the first line of this document, this is the url of this document.
	10. Print the number of matches for the query, i.e. the number of (docId, score) pairs. Print the document Ids
	    and their scores ranked from highest to lowest. Along with the document Ids print the url obtained from step 9.
5. Free memory for the index structure and any other structures used

### Dataflow through modules

1. *main* parses/validates parameters, calls functions from *index* module
2. Functions from *index* module initialize a new index and returns back to *main*
3. *main* passes parameters to *index_load*
4. *index_load* loads information from indexFilename into the index. It returns control to *main*
5. *main* starts reading standard input, it calls *prompt* every time it reads a line of stdin
6. *prompt* prints a prompt to stdin if it is an interactive user. Then returns control to *main*
7. *main* passes a line from stdin to *clean_input*
8. *clean_input*, takes this line and makes sure it follows proper query syntax and standardizes its format,
   after which it returns control to *main*
9. *main* then passes the cleaned query and the index to *calculate_scores*
10. *calculate_scores* finds and calculates the scores for all documents that satisfy the query. It calls
    on *or_counters* and *and_counters* to perform the 'or' and 'and' operations. These stores are returned to
    *main*.
11. *main* passes the scores to *sort_counters*, which sorts the scores from highest to lowest. These sorted scores
    are returned to *main*.
12. *main* calls *getPageURL* to get the url for each scored document.
13. *main* prints the sorted scores with their respective urls.

### Major data structures

The *index* module provides the index structure...

1. index, mapping of words and (docId, count) pairs

The *index* module uses a few helper modules which provide the following data structures:

2. hashtable of words and counters (indirectly used by index)

3. set of words and counters (indirectly used by index)

4. counters which keep a count of a docId

### Testing Plan

Test the querier through `testing.sh` and ensure that each output produced matches expectations through close inspection. Testing.sh will have processes that will do the following:

1. Test querier with various forms of incorrect command-line arguments to see if they are properly parsed and validated.

2. Test querier with various forms of invalid queries including:
	- non-alphabetic, non-space characters within query
	- 'and' and 'or' operations appearing adjacent to each other in a query
	- 'and' and 'or' operations appearing at the end and beginning in a query
3. Test querier with input that has capitalization and extra spaces, making sure these do not affect the query.
4. Test querier with an empty query.
5. Test querier with a query that has no documents that satisfy it.
6. Test querier with a large but still managable index multiple times using the `fuzzquery.c` program. This is used to test
   the functionality of the program. The size needs to be managable so that manual inspection of the scores is not too    
   tedious, but large enough that the calculation of the scores is not trivial.
7. Test querier with a very large index to test its speed in answering queries involving larger data sets.
