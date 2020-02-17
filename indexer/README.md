# CS50 Lab 4
## Jacob Werzinsky, CS50 Winter 2020

### crawler

Crawler gathers information which includes the html and url of webpages starting from a given url. The crawler
starts at this url and follows all links within the url's webpage to other sites and stores their information as well. 
It then searches for urls from those sites and continues this process until all links are exhausted or a max depth, defined
by the amount of links a page is away from the starting webpage, is reached. This implementation will only search webpages
within http://old-www.cs.dartmouth.edu. Saves results in the form of numbered files with the url,html and depth to a specified directory. 

### Usage

crawler [seedURL] [pageDirectory] [maxDepth]

*Example*:

crawler http://old-www.cs.dartmouth.edu directory 5 

### Assumptions

The design spec did not specify how we should handle the case where there are already numbered files within
the directory passed by the caller. For example, the case where someone wants to use the same directory for
different calls of crawler with various seed URLs. So I assumed that a user would prefer to keep any numbered
files within the passed directory. This mainly pertains to the pageSaver() function.

I assumed that the crawling area was not going to be too big, about the size of the testing examples, so the 
hashtable used in the program contains only 200 sets which on larger scales will run increasingly slower.

### Compilation
To comple crawler, do: `make crawler.o`.

See [TESTING](TESTING.md) for details of testing and an example test run.
