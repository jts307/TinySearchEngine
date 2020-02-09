#!/bin/bash
#
# testing.sh - used for testing the program crawler.c 
# 
# Created by Jacob Werzinsky, CS50, 21 January 2020


# making testing directories, program exits without testing if it cannot create a directory
# or if it does not already exist
mkdir "testing"
if [ ! -d "testing" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi
mkdir "testing/letters-depth-0"
if [ ! -d "testing/letters-depth-0" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi
mkdir "testing/letters-depth-1"
if [ ! -d "testing/letters-depth-1" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi
mkdir "testing/letters-depth-2"
if [ ! -d "testing/letters-depth-2" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi
mkdir "testing/letters-depth-3"
if [ ! -d "testing/letters-depth-3" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi
mkdir "testing/letters-depth-4"
if [ ! -d "testing/letters-depth-4" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi
mkdir "testing/letters-depth-5"
if [ ! -d "testing/letters-depth-5" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi
mkdir "testing/v2-letters-depth-0"
if [ ! -d "testing/v2-letters-depth-0" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi
mkdir "testing/v2-letters-depth-1"
if [ ! -d "testing/v2-letters-depth-1" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi
mkdir "testing/v2-letters-depth-2"
if [ ! -d "testing/v2-letters-depth-2" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi
mkdir "testing/v2-letters-depth-3"
if [ ! -d "testing/v2-letters-depth-3" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi
mkdir "testing/v2-letters-depth-4"
if [ ! -d "testing/v2-letters-depth-4" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi

mkdir "testing/v2-letters-depth-5"
if [ ! -d "testing/v2-letters-depth-5" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi

mkdir "testing/wikipedia-depth-0"
if [ ! -d "testing/wikipedia-depth-0" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi

mkdir "testing/wikipedia-depth-1"
if [ ! -d "testing/wikipedia-depth-1" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi

mkdir "testing/wikipedia-depth-2"
if [ ! -d "testing/wikipedia-depth-2" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi
mkdir "testing/classwebsite"
if [ ! -d "testing/classwebsite" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi


# Testing program with various forms of incorrect parameters

./crawler 
echo "$?"
./crawler wrong wrong wrong
echo "$?"
./crawler wrong wrong 1
echo "$?"
./crawler wrong testing wrong
echo "$?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html wrong wrong
echo "$?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing a121a
echo "$?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing wrong
echo "$?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing1 2
echo "$?"
./crawler http://old-www.cs.dartmh.edu/~cs50/data/tse/letters/index testing 1
echo "$?" 
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing -1
echo "$?"
# should produce 2 output files in testing
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing 1a
echo "$?"


# Testing crawler with a seedURL pointing to a non-existent server
./crawler http://old-www.cs.doesnotexistdnedne.edu/~cs50/data/tse/letters/index.html testing/junk 1
echo "$?"

# Testing crawler with a seedURL pointing to a non-internal server
./crawler https://en.wikipedia.org/wiki/%22Hello,_World!%22_program testing/junk 1
echo "$?"

# Testing with an existing server but non-existing page
./crawler http://old-www.cs.dartmouth.edu/~cs50/today testing/junk 1
echo "$?"

# Testing the crawler at various depths (0-5) in the cs letters page
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-0 0
echo "$?"

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-1 1
echo "$?"

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-2 2
echo "$?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-3 3
echo "$?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-4 4
echo "$?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-5 5
echo "$?"
# Testing the crawler at the cs letters set of pages but from a different starting point
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html testing/v2-letters-depth-0 0
echo "$?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html testing/v2-letters-depth-1 1
echo "$?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html testing/v2-letters-depth-2 2
echo "$?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html testing/v2-letters-depth-3 3
echo "$?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html testing/v2-letters-depth-4 4
echo "$?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html testing/v2-letters-depth-5 5
echo "$?"
# Testing the crawler with the wikipedia page at depths 0, 1 and 2
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ testing/wikipedia-depth-0 0
echo "$?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ testing/wikipedia-depth-1 1
echo "$?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ testing/wikipedia-depth-2 2
echo "$?"
# Testing it on the cs50 class website with a higher depth
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html testing/classwebsite-depth-20 20
echo "$?"

exit 0
