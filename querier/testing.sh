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
make
cd ../querier

# making testing directories, program exits without testing if it cannot create a directory
# or if it does not already exist
############################################################################################
mkdir "testing"
if [ ! -d "testing" ]
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

# Creating crawler files and index at depth 5 in the cs letters page
#######################################################################
../crawler/crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html testing/letters-depth-5 5 > /dev/null 2>&1
../indexer/indexer testing/letters-depth-5 testing/indexerResults/letters5Index > /dev/null 2>&1

# Creating crawler files and index with the wikipedia page at depth 2
#####################################################################
#../crawler/crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ testing/wikipedia-depth-2 2 > /dev/null 2>&1
#../indexer/indexer testing/wikipedia-depth-2 testing/indexerResults/wikipedia2Index > /dev/null 2>&1

# Testing programs with various forms of incorrect parameters
############################################################

# not three arguments
./querier testing
echo "Exit status of not three arguments: $?"

# Testing with non-existent directory
./querier hello testing/indexerResults/letters5Index
echo "Exit status of non-existent directory: $?"

# Testing with non-crawler directory
./querier testing testing/indexerResults/letters5Index
echo "Exit status of non-crawler directory: $?"

# Testing with non-readable directory
chmod -r testing/ 
./querier testing/ testing/indexerResults/letters5Index 
echo "Exit status of non-readable directory: $?"
chmod +r testing/

# Testing with non-readable file
chmod -r testing/indexerResults/letters5Index
./querier testing/letters-depth-5 testing/indexerResults/letters5Index
echo "Exit status of non-readable file: $?"
chmod +r testing/indexerResults/letters5Index

# Testing with non-valid file path
./querier testing/letters-depth-5 testing5/indexfile
echo "Exit status of non-valid file path: $?"

# Testing with empty crawler directory
touch testing/.crawler
./fuzzquery testing/indexerResults/letters5Index 3 1 | ./querier testing/ testing/indexerResults/letters5Index
echo "Exit status of empty crawler directory: $?"
rm -f testing/.crawler

# Testing various forms of query imputs:
#	- invalid characters, i.e. non-alphabetic and non-space.
#	- operators at the beginning and end of queries
#	- operators adjacent to each other in a query
#	- empty query
#	- capitalized characters
#	- extra spaces between words
# 	- a query where no documents satisfy the query
# These tests are all contained in the testquery file
./querier testing/letters-depth-5 testing/indexerResults/letters5Index < queryTest
echo "Exit status of test with testquery file: $?"

# Testing the querier with a series of queries for letter-depth-5
# to test its functionality
./fuzzquery testing/indexerResults/letters5Index 20 2 | ./querier testing/letters-depth-5 testing/indexerResults/letters5Index
echo "Exit status of letters-index-5: $?"

# Testing the querier with a series of queries for wikipedia-depth-2
# To test the speed of queries on larger indexes
#./fuzzquery testing/indexerResults/wikipedia2Index 20 1 | ./querier testing/wikipedia-depth-2 testing/indexerResults/wikipedia2Index
#echo "Exit status of wikipedia-index-2: $?"

exit 0
