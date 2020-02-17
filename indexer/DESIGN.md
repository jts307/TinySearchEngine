# TSE Indexer Design Spec

## Jacob Werzinsky, CS50, February 2020

### User Interface

The indexer's only interface with the user is on the command line, which must have two arguments and the following usage:

```bash
indexer pageDirectory IndexFilename
```

For example,

```shell 
$ ./indexer directory file
```









Testing Plan

Test the indexer and indextest programs through testing.sh and ensure that each output produced matches expectations. Testing.sh will have processes that will do the following:


1. Test each program with various forms of incorrect command-line arguments to see if they are properly parsed and validated.

2. Test indexer with samples that contain words that repeat a lot, have mixed capitalization, non-alphabetic characters,  and contain words with less than 3 characters.

3. Test indexer with a relatively  small amount of crawler files in pageDirectory.

4. Test indexer with a relatively large amount of crawler files in pageDirectory.

5. Test on empty crawler directory

6. Test indextest with an empty file.

7. Test indextest with the index files produced by the indexer. Make sure the output matches the input exactly using the provided indexsort.awk file.


No need to test wikipedia 2 or scrape 2, leave commented out, make note of it in testing.md

Create directory called crawler-output, don’t upload it but make your thing test it

Do not upload indexes produced by crawler

indexer:
	1	Process and validate command-line parameters
	2	Initialize data structure index
	3	index_build(directory, index)
	4	index_save(file, index);
	5	clean up data structures
indextest:
	1	Process and validate command-line parameters
	2	Initialize data structure index
	3	index_load(file1, index)
	4	index_save(file2, index)
	5	clean up data structures
