# CS50 Lab 4
## Jacob Werzinsky, CS50 Winter 2020

### pagedir

The pagedir module handles operations that involve writing files to the pageDirectory. This includes dummy files 
and webpage files created by crawler.

### Usage

The *pagedir* module, defined in `pagedir.h` and implemented in `pagedir.c`:

```c
/* Checks if a directory exists and is writable by attempting to write 
 * '.crawler' dummy file in directory. Note that .crawler remains in 
 * directory after function is done.
 * parameters:
 *   pageDir - pointer to the path of directory to be tested
 * returns:
 *   int with value of 0 if directory exists and is writable, 1 otherwise.
 * When pageDir is NULL or failed memory allocation, 
 * then an error is written to stderr and it exits with positive status.
 */
int isValidDirectory(const char *pageDir);

/* Takes a webpage type and writes its information into a page file with the
 * following format:
 *      Line 1: webpage URL
 *      Line 2: webpage depth
 *      Line 3 and onward: webpage HTML
 * parameters:
 *   pageDir - directory page file will be saved to
 *   webpage - pointer to webpage type whose information will be written to a file.
 * returns:
 *   returns 0 on success and positive value on any error.
 *   Error statuses:
 *      1 - NULL pageDir
 *      2 - NULL wp
 *      3 - NULL wp->url
 *      4 - NULL wp->html
 *      5 - Negative wp->depth
 *      6 - failed to create filePath String
 *      7 - Error creating or writing to page file
 *      8 - Error allocating memory for stringId         
 */
int pageSaver(const char *pageDir, webpage_t *wp);
```

### Assumptions

The design spec did not specify how we should handle the case where there are already numbered files within
the directory passed by the caller. For example, the case where someone wants to use the same directory for
different calls of crawler with various seed URLs. So I assumed that a user would prefer to keep any numbered
files within the passed directory. This mainly pertains to the pageSaver() function.

### Compilation

To compile common archive, do: `make common.a`.
To comple pagedir module, do: `make pagedir.o`.

See [TESTING](../crawler/TESTING.md) for details of testing and an example test run.
