# CS50 Lab 6
## Jacob Werzinsky, CS50 Winter 2020

### querier

The querier takes the output files produced by the [crawler](../crawler/README.md) and the indexes produced by the [indexer](../indexer/README.md)



produces a mapping from each 
unique word within them to (documentId, count) pairs, where documentId is the name of a file and count is how many times 
the matching word appears in that file. It then takes this mapping, made using the [index module](../common/index.h), and stores it into a file.

#### Usage

querier [pageDirectory] [IndexFilename]

*Example*:

./querier directory file

### Assumptions

- When printing the documents ids and their scores to standard output, I assumed a maximum of four digits. Any more will 
  cause the format of the printf statement to look not appealing.

### Compilation
To compile querier, do: `make querier.o`.

To compile fuzzquerier, do: `make fuzzquerier.o`

To compile both, do: `make` or `make all`

See [TESTING](TESTING.md) for details of testing and an example test run.
