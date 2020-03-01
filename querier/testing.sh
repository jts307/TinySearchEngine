DATA_PARENT_DIR=../.. # modify path to specfic data directory

# build querier
# make

# # bad parameters
./querier
./querier ../data/data3
./querier ../data/data3 ../data/index3 more
./querier ../data/data
./querier ../data/data ../data/index3
./querier . ../data/index3
./querier ../data/data3 /indexFile


# ### Small test cases (12 = 1 * 12, 1 point for each)
# #### T1: check # of arguments
./querier $DATA_PARENT_DIR/data/letters-depth-6/ ~cs50/data/tse-output/letters-index-6 data

#### T2: check crawler directory
./querier ../common $DATA_PARENT_DIR/data/letters-index-6

#### T3: check existence of index file
./querier $DATA_PARENT_DIR/data/letters-depth-6/ ../index


### Syntax errors in query input (1 point each)
# Run the querier with any folder names, e.g
./querier $DATA_PARENT_DIR/data/letters-depth-6/ $DATA_PARENT_DIR/data/tse-output/letters-index-6 <<EOF
and Dartmouth
or Dartmouth
Dartmouth or
Dartmouth and
MIT and or Dartmouth
Boston or and Hanover
computer science 60
I love computer science!
Tiny Search-engine
EOF


### Big test cases (18 = 2 * 9, 2 points each, focus on ranking and whether the result is complete)
#### T13-T21
./querier $DATA_PARENT_DIR/data/toscrape-depth-1/ $DATA_PARENT_DIR/data/toscrape-index-1 <<EOF
years
book or story
book and history
art price
music band or painting
modern music or classical poem
political nonfiction or modern art or humor story
eat fat get thin or the time keeper or the black maria
paris kitchen recipes baking or white cat bear whale or best reviews
EOF


# fuzz testing

#./fuzzquery $DATA_PARENT_DIR/data/toscrape-index-2 10000 0 | ./querier $DATA_PARENT_DIR/data/toscrape-depth-2 $DATA_PARENT_DIR/data/toscrape-index-2 > /dev/null && echo success || echo failed


# Memory-leak testing
VALGRIND='valgrind --leak-check=full --show-leak-kinds=all'

$VALGRIND  ./querier $DATA_PARENT_DIR/data/toscrape-depth-2 $DATA_PARENT_DIR/data/toscrape-index-2 >/dev/null

#$VALGRIND  ./fuzzquery $DATA_PARENT_DIR/data/toscrape-index-2 100 0 >/dev/null

#./fuzzquery $DATA_PARENT_DIR/data/toscrape-index-2 10000 0 | $VALGRIND ./querier $DATA_PARENT_DIR/data/toscrape-depth-2 $DATA_PARENT_DIR/data/toscrape-index-2 > /dev/null
DATA_PARENT_DIR=../.. # modify path to specfic data directory

# build querier
# make

# # bad parameters
# ./querier
# ./querier ../data/data3
# ./querier ../data/data3 ../data/index3 more
# ./querier ../data/data
# ./querier ../data/data ../data/index3
# ./querier . ../data/index3
# ./querier ../data/data3 /indexFile


# ### Small test cases (12 = 1 * 12, 1 point for each)
# #### T1: check # of arguments
./querier $DATA_PARENT_DIR/data/letters-depth-6/ ~cs50/data/tse-output/letters-index-6 data

#### T2: check crawler directory
./querier ../common $DATA_PARENT_DIR/data/letters-index-6

#### T3: check existence of index file
./querier $DATA_PARENT_DIR/data/letters-depth-6/ ../index


### Syntax errors in query input (1 point each)
# Run the querier with any folder names, e.g
./querier $DATA_PARENT_DIR/data/letters-depth-6/ $DATA_PARENT_DIR/data/tse-output/letters-index-6 <<EOF
and Dartmouth
or Dartmouth
Dartmouth or
Dartmouth and
MIT and or Dartmouth
Boston or and Hanover
computer science 60
I love computer science!
Tiny Search-engine
EOF


### Big test cases (18 = 2 * 9, 2 points each, focus on ranking and whether the result is complete)
#### T13-T21
./querier $DATA_PARENT_DIR/data/toscrape-depth-1/ $DATA_PARENT_DIR/data/toscrape-index-1 <<EOF
years
book or story
book and history
art price
music band or painting
modern music or classical poem
political nonfiction or modern art or humor story
eat fat get thin or the time keeper or the black maria
paris kitchen recipes baking or white cat bear whale or best reviews
EOF


# fuzz testing

#./fuzzquery $DATA_PARENT_DIR/data/toscrape-index-2 10000 0 | ./querier $DATA_PARENT_DIR/data/toscrape-depth-2 $DATA_PARENT_DIR/data/toscrape-index-2 > /dev/null && echo success || echo failed


# Memory-leak testing
VALGRIND='valgrind --leak-check=full --show-leak-kinds=all'

$VALGRIND  ./querier $DATA_PARENT_DIR/data/toscrape-depth-2 $DATA_PARENT_DIR/data/toscrape-index-2 >/dev/null

#$VALGRIND  ./fuzzquery $DATA_PARENT_DIR/data/toscrape-index-2 100 0 >/dev/null

#./fuzzquery $DATA_PARENT_DIR/data/toscrape-index-2 10000 0 | $VALGRIND ./querier $DATA_PARENT_DIR/data/toscrape-depth-2 $DATA_PARENT_DIR/data/toscrape-index-2 > /dev/null
