# Testing for crawler and the pagedir module

To test these modules simply run the following command:

```bash
make test &> testing.out
```

The command will produce a "testing" directory and a series of directories in it 
which each contain results produced by using crawler and the pagedir module. Testing.out 
will contain all the exit codes and error messages produced by the tests. There is a provided
sample testing.out and testing directory that was produced using the above command.

Notes: 
-Before running testing.sh delete the sample testing directory or the directory will have
two copies of the same test results which might make it seem like the program behaved differently than
intended. You can run `make tclean` to do this.

-Some of the tests take a long time to run, particularly wikipedia-depth-2, wikipedia-depth-3 and toscrape-depth-2,
so it is recommended to either increase the hashtable size in crawler.c to maybe speed up performance or to just 
comment out these tests entirely.    
