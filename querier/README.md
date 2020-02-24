# CS50 Lab 6
## Jacob Werzinsky, CS50 Winter 2020

### querier

The querier takes the output files produced by the [crawler](../crawler/README.md) and the indexes produced by the [indexer](../indexer/README.md), and uses them to answer queries imputted by a user from the standard input. Each query is a series of words seperated by 'and' and 'or' operators. The querier finds the documents/webpages that satisfy a query and reports these back with their scores, ranked from highest score to lowest. Scores are based on the frequency in which individual words in a query appear within a document. The 'and' operation indicates that words must appear together within a document to be counted, it goes only between words. The 'or' operators goes between words and/or 'and' sequences, indicating that both operands on either side of an 'or' have their scores summed.

### Functionality

I implemented my querier with full functionality, i.e. it meets all specs:
  -It prints all documents that satisfy a query in decreasing order by score.
  -Supports the 'and' and 'or' operations with 'and' having precedence over 'or'.

### Usage

querier [pageDirectory] [IndexFilename]

*Example*:

./querier directory file

### Assumptions

- When printing the documents ids and their scores to standard output, I assumed a maximum of four digits for each field. Any   
  more will not be printed. This can be easily changed by increasing the digit padding the printf statement for the documents    
  and their scores uses.

### Compilation
To compile querier, do: `make querier.o`.

To compile fuzzquerier, do: `make fuzzquerier.o`.

To compile both, do: `make` or `make all`.

See [TESTING](TESTING.md) for details of testing and an example test run.
