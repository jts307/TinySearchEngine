# Testing for querier

To test the module simply run the following command:

```bash
make test &> testing.out
```

The command will produce a "testing" directory and three directories within it. Two of the directories will be
used as crawler directories, namely 'letters-depth-5' and 'wikipedia-depth-2'. The crawler will be run on the cs letters
page with a depth of 5 and the wikipedia page with a depth of 2, and the results of these crawls will be stored in these
two directories. A third directory will also be created called 'indexerResults' which will contain the indexes of 'letters-depth-5' and 'wikipedia-depth-2', which would be produced by the indexer. Using the produced crawler and indexer data files 
the test will then call querier to query a set of randomized searches produced by `fuzzquery.c` on the letters and wikipedia pages. On top of this, the querier will query a manually made set of queries in the file `queryTest` on the letters page. Testing.out will contain all the exit statuses, error messages and query answers produced by the querier. There is a sample testing.out which is produced by this command.

Note: 
- Producing the crawler wikipedia-depth-2 files takes a long time to do so I commented this out by default. However, the 
  testing.out file provided was produced with this uncommented.
