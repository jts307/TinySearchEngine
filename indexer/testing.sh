#!/bin/bash
#
# Testing script for indexer.c and indextest.c
# Author: Jacob Werzinsky
# Date: Feb 8, 2020
#
# usage: bash -v testing.sh
# 
# Created by Jacob Werzinsky, CS50, Winter 2020

cd ../crawler
make
cd ../indexer

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

mkdir "testing/indexerResults"
if [ ! -d "testing/indexerResults" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi

mkdir "testing/indextestResults"
if [ ! -d "testing/indextestResults" ]
then 
	echo 1>&2 "Error creating directory for testing results"
	exit 1
fi

touch "testing/dummyIndexFile"
chmod -w testing/dummyIndexFilew
touch "testing/dummyIndexFile2"
chmod -r testing/dummyIndexFiler

# Creating crawler files at various depths (0-5) in the cs letters page
#######################################################################
../crawler/crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-0 0
../crawler/crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-1 1
../crawler/crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-2 2
../crawler/crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-3 3
../crawler/crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-4 4
../crawler/crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-5 5

# Creating crawler files with the wikipedia page at depths 0, 1 and 2
#####################################################################
../crawler/crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ testing/wikipedia-depth-0 0
../crawler/crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ testing/wikipedia-depth-1 1
../crawler/crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ testing/wikipedia-depth-2 2

# Testing programs with various forms of incorrect parameters
############################################################

# Indexer
##########
# Testing with non-existent directory
./indexer hello IndexFile
echo "Exit status of non-existent directory: $?"

# Testing with non-crawler directory
./indexer testing IndexFile
echo "Exit status of non-crawler directory: $?"

# Testing with non-readable directory
chmod -r testing/letters-depth-0
./indexer testing/letters-depth-0 IndexFile
echo "Exit status of non-readable directory: $?"
chmod +r testing/letters-depth-0

# Testing with non-writable file
./indexer testing/letters-depth-0 testing/dummyIndexFilew
echo "Exit status of non-writable file: $?"

# Testing with non-valid file path
./indexer testing/letters-depth-0 testing5/indexfile
echo "Exit status of non-valid file path: $?"

# Testing with empty crawler directory
touch testing/letters-depth-0/.crawler
./indexer testing/letters-depth-0 testing/indexerResults/indexEmpty
echo "Exit status of empty crawler directory: $?"

# IndexTest
###########
# Testing with non-readable old index file
./indextest testing/dummyIndexFiler indexFile
echo "Exit status of non-readable old index file: $?"

# Testing with non-valid old index file path
./indextest testing5/indexfile indexFile
echo "Exit status of non-valid old index file path: $?"

chmod +r testing/dummyIndexFiler

# Testing with non-writable new index file
./indextest testing/dummyIndexFiler testing/dummyIndexFilew
echo "Exit status of non-writable new index file: $?"

# Testing with non-valid new index file path
./indextest testing/dummyIndexFiler testing5/index
echo "Exit status of non-valid new index file path: $?"

# Testing on empty old Index file
./indextest testing/indexerResults/indexEmpty testing/indextestResults/indexEmpty
echo "Exit status of empty old index file: $?"

# Testing indexer on relatively small amount of crawler files using the letters directories
# They also contain words that repeat, have mixed capitalization, non-alphabetic characters,
# and have words that have less than 3 characters.
###########################################################################################
./indexer testing/letters-depth-0 testing/indexerResults/lettersIndex-0
echo "Exit status of index of letters-depth-0: $?"
./indexer testing/letters-depth-1 testing/indexerResults/lettersIndex-1
echo "Exit status of index of letters-depth-1: $?"
./indexer testing/letters-depth-2 testing/indexerResults/lettersIndex-2
echo "Exit status of index of letters-depth-2: $?"
./indexer testing/letters-depth-3 testing/indexerResults/lettersIndex-3
echo "Exit status of index of letters-depth-3: $?"
./indexer testing/letters-depth-4 testing/indexerResults/lettersIndex-4
echo "Exit status of index of letters-depth-4: $?"
./indexer testing/letters-depth-5 testing/indexerResults/lettersIndex-5
echo "Exit status of index of letters-depth-5: $?"

# Testing indexer on relatively large amount of crawler files using the wikipedia directories
##############################################################################################
./indexer testing/wikipedia-depth-0 testing/indexerResults/wikipediaIndex-0
echo "Exit status of index of wikipedia-depth-0: $?"
./indexer testing/wikipedia-depth-1 testing/indexerResults/wikipediaIndex-1
echo "Exit status of index of wikipedia-depth-1: $?"
./indexer testing/wikipedia-depth-2 testing/indexerResults/wikipediaIndex-2
echo "Exit status of index of wikipedia-depth-2: $?"

# Testing indextest on results of indexer
#########################################
./indextest testing/indexerResults/lettersIndex-0 testing/indextestResults/lettersIndex-0
echo "Exit status of copying letters-depth-0: $?"
./indextest testing/indexerResults/lettersIndex-1 testing/indextestResults/lettersIndex-1
echo "Exit status of copying letters-depth-1: $?"
./indextest testing/indexerResults/lettersIndex-2 testing/indextestResults/lettersIndex-2
echo "Exit status of copying letters-depth-2: $?"
./indextest testing/indexerResults/lettersIndex-3 testing/indextestResults/lettersIndex-3
echo "Exit status of copying letters-depth-3: $?"
./indextest testing/indexerResults/lettersIndex-4 testing/indextestResults/lettersIndex-4
echo "Exit status of copying letters-depth-4: $?"
./indextest testing/indexerResults/lettersIndex-5 testing/indextestResults/lettersIndex-5
echo "Exit status of copying letters-depth-5: $?"
./indextest testing/indexerResults/wikipediaIndex-0 testing/indextestResults/wikipediaIndex-0
echo "Exit status of copying wikipedia-depth-0: $?"
./indextest testing/indexerResults/wikipediaIndex-1 testing/indextestResults/wikipediaIndex-1
echo "Exit status of copying wikipedia-depth-1: $?"
./indextest testing/indexerResults/wikipediaIndex-2 testing/indextestResults/wikipediaIndex-2
echo "Exit status of copying wikipedia-depth-2: $?"


gawk -f indexsort.awk testing/indextestResults/lettersIndex-0 | sort > testing/indextestResults/lettersIndex-0.sorted
gawk -f indexsort.awk testing/indextestResults/lettersIndex-1 | sort > testing/indextestResults/lettersIndex-1.sorted
gawk -f indexsort.awk testing/indextestResults/lettersIndex-2 | sort > testing/indextestResults/lettersIndex-2.sorted
gawk -f indexsort.awk testing/indextestResults/lettersIndex-3 | sort > testing/indextestResults/lettersIndex-3.sorted
gawk -f indexsort.awk testing/indextestResults/lettersIndex-4 | sort > testing/indextestResults/lettersIndex-4.sorted
gawk -f indexsort.awk testing/indextestResults/lettersIndex-5 | sort > testing/indextestResults/lettersIndex-5.sorted
gawk -f indexsort.awk testing/indextestResults/wikipediaIndex-0 | sort > testing/indextestResults/wikipediaIndex-0.sorted
gawk -f indexsort.awk testing/indextestResults/wikipediaIndex-1 | sort > testing/indextestResults/wikipediaIndex-1.sorted
gawk -f indexsort.awk testing/indextestResults/wikipediaIndex-2 | sort > testing/indextestResults/wikipediaIndex-2.sorted


gawk -f indexsort.awk testing/indexerResults/lettersIndex-0 | sort > testing/indexerResults/lettersIndex-0.sorted
gawk -f indexsort.awk testing/indexerResults/lettersIndex-1 | sort > testing/indexerResults/lettersIndex-1.sorted
gawk -f indexsort.awk testing/indexerResults/lettersIndex-2 | sort > testing/indexerResults/lettersIndex-2.sorted
gawk -f indexsort.awk testing/indexerResults/lettersIndex-3 | sort > testing/indexerResults/lettersIndex-3.sorted
gawk -f indexsort.awk testing/indexerResults/lettersIndex-4 | sort > testing/indexerResults/lettersIndex-4.sorted
gawk -f indexsort.awk testing/indexerResults/lettersIndex-5 | sort > testing/indexerResults/lettersIndex-5.sorted
gawk -f indexsort.awk testing/indexerResults/wikipediaIndex-0 | sort > testing/indexerResults/wikipediaIndex-0.sorted
gawk -f indexsort.awk testing/indexerResults/wikipediaIndex-1 | sort > testing/indexerResults/wikipediaIndex-1.sorted
gawk -f indexsort.awk testing/indexerResults/wikipediaIndex-2 | sort > testing/indexerResults/wikipediaIndex-2.sorted


echo "Differences between indexes if any:"

diff testing/indextestResults/lettersIndex-0.sorted testing/indexerResults/lettersIndex-0.sorted
diff testing/indextestResults/lettersIndex-1.sorted testing/indexerResults/lettersIndex-1.sorted
diff testing/indextestResults/lettersIndex-2.sorted testing/indexerResults/lettersIndex-2.sorted
diff testing/indextestResults/lettersIndex-3.sorted testing/indexerResults/lettersIndex-3.sorted
diff testing/indextestResults/lettersIndex-4.sorted testing/indexerResults/lettersIndex-4.sorted
diff testing/indextestResults/lettersIndex-5.sorted testing/indexerResults/lettersIndex-5.sorted
diff testing/indextestResults/wikipediaIndex-0.sorted testing/indexerResults/wikipediaIndex-0.sorted 
diff testing/indextestResults/wikipediaIndex-1.sorted testing/indexerResults/wikipediaIndex-1.sorted
diff testing/indextestResults/wikipediaIndex-2.sorted testing/indexerResults/wikipediaIndex-2.sorted




exit 0
