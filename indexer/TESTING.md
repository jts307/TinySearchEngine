# Testing for indexer and indextest

To test these modules simply run the following command:

```bash
make test &> testing.out
```

The command will produce a "testing" directory and a series of directories in it 
which each contain results produced by using crawler. There will also be two other directories
named indexerResults and indextestResults which will contains indexes produced by the indexer and indextest
respectively out of the crawler produced files. Testing.out will contain all the exit codes, error messages
and progress indicators produced by the tests. There is a sample testing.out which is produced by this command.

Note: 
- Producing the crawler wikipedia-depth-2 files takes a long time to run so I commented this out by default. The 
  testing.out file provided was produced with this uncommented.
