#!/bin/bash
#
# Testing script for crawler.c and pagedir module
# Author: Jacob Werzinsky
# Date: Feb 8, 2020
#
# usage: bash -v testing.sh
# 
# Created by Jacob Werzinsky, CS50, Winter 2020


# making testing directories, program exits without testing if it cannot create a directory
# or if it does not already exist
############################################################################################
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

mkdir "testing/toscrape-depth-2"
if [ ! -d "testing/toscrape-depth-2" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi

mkdir "testing/wikipedia-depth-3"
if [ ! -d "testing/wikipedia-depth-3" ]
then
        echo 1>&2 "Error creating directory for testing results"
        exit 1
fi


# Testing program with various forms of incorrect parameters
#############################################################

# Testing with zero arguments
./crawler
echo "Exit Code for no arguments: $?"

# Testing with two arguements
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing 
echo "Exit code for two arguments: $?"

# Testing with four arguments
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing 1 1
echo "Exit code for four arguments: $?"

# Testing with 3 invalid arguments
./crawler wrong wrong wrong
echo "Exit code for 3 invalid arguments: $?"

# Testing with only valid depth
./crawler wrong wrong 1
echo "Exit code for only valid depth: $?"

# Testing with only valid directory
./crawler wrong testing wrong
echo "Exit code for only valid directory: $?"

# Testing with only valid url 
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html wrong wrong
echo "Exit code for only valid url: $?"

# Testing with invalid depth
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing a121a
echo "Exit code for invalid depth case 1: $?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing wrong
echo "Exit code for invalid depth case 2: $?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing -1
echo "Exit code for invalid depth case 3: $?"

# Testing with invalid directory
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing1 2
echo "Exit code for invalid directory: $?"

# Testing with invalid url
./crawler http://old-www.cs.dartmh.edu/~cs50/data/tse/letters/index testing 1
echo "Exit code for invalid url: $?" 

# Testing with depth that contains characters after its imputted number 
# should produce two files in testing directory
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing 1word
echo "Exit code for valid depth case 1: $?"

# Testing what happens when I try to write to the same directory twice
# running same command directly above again with different word. Output 
# in testing should be 2 more files for a total of 4 files labeled 1-4.
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing 1word
echo "Exit code for valid depth case 2: $?"

# Testing crawler with a seedURL pointing to a non-existent server
##################################################################
./crawler http://old-www.cs.doesnotexistdnedne.edu/~cs50/data/tse/letters/index.html testing 1
echo "Exit code for non-existent server: $?"

# Testing crawler with a seedURL pointing to a non-internal server
##################################################################
./crawler https://en.wikipedia.org/wiki/%22Hello,_World!%22_program testing 1
echo "Exit code for non-internal server: $?"

# Testing with an existing server but non-existing page
#######################################################
./crawler http://old-www.cs.dartmouth.edu/~cs50/today testing 1
echo "Exit code for non-existent page: $?"

# Testing the crawler at various depths (0-5) in the cs letters page
####################################################################
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-0 0
echo "Exit code for letters depth 0: $?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-1 1
echo "Exit code for letters depth 1: $?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-2 2
echo "Exit code for letters depth 2: $?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-3 3
echo "Exit code for letters depth 3: $?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-4 4
echo "Exit code for letters depth 4: $?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-5 5
echo "Exit code for letters depth 5: $?"

# Testing the crawler at the cs letters set of pages but from a different starting point
########################################################################################
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html testing/v2-letters-depth-0 0
echo "Exit code for v2-letters depth 0: $?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html testing/v2-letters-depth-1 1
echo "Exit code for v2-letters depth 1: $?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html testing/v2-letters-depth-2 2
echo "Exit code for v2-letters depth 2: $?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html testing/v2-letters-depth-3 3
echo "Exit code for v2-letters depth 3: $?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html testing/v2-letters-depth-4 4
echo "Exit code for v2-letters depth 4: $?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/E.html testing/v2-letters-depth-5 5
echo "Exit code for v2-letters depth 5: $?"

# Testing the crawler with the wikipedia page at depths 0, 1 and 2
##################################################################
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ testing/wikipedia-depth-0 0
echo "Exit code for wikipedia depth 0: $?"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ testing/wikipedia-depth-1 1
echo "Exit code for wikipedia depth 1: $?"

# Note: The following three tests take a long time to run
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ testing/wikipedia-depth-2 2
echo "Exit code for wikipedia depth 2: $?"

# Testing on toscrape and on the wikipedia page at a higher depth
#################################################################
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html testing/toscrape-depth-2 2
echo "Exit code for toscrape depth 2: $?"

./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ testing/wikipedia-depth-3 3
echo "Exit code for wikipedia depth 3: $?"



exit 0
