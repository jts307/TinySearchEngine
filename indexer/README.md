# CS50 Lab 4
## Jacob Werzinsky, CS50 Winter 2020

### indexer

The indexer takes the output files produced by the [crawler](../crawler/README.md) and produces a mapping from each 
unique word within them to (documentId, count) pairs, where documentId is the name of a file and count is how many times 
the matching word appears in that file. It then takes this mapping, made using the [index module](../common/index.h), and stores it into a file.

#### Usage

indexer [pageDirectory] [IndexFilename]

*Example*:

./indexer directory file

### indextest

The indextest program tests the index structure by first loading data from an already made index file into the structure
and then resaving the data from the index to a new file.

#### Usage

indextest [oldIndexFilename] [newIndexFilename]

*Example*:

./indextest file1 file2

### Assumptions

None beyond those stated in the lab assignment.

### Compilation
To compile indexer, do: `make indexer.o`.
To compile indextest, do: `make indextest.o`.
To make both, do: `make` or `make all`.

See [TESTING](TESTING.md) for details of testing and an example test run.
