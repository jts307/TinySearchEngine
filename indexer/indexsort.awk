#!/usr/local/bin/gawk -f
# Sort the document pairs on each line of a TSE index file,
# and pipe output to `sort` so we sort the lines alphabetically.
#
# usage example: ./indexsort.awk indexFilename > indexFilename.sorted
# if that does not work, you may wish to try
#   gawk -f indexsort.awk indexFilename > indexFilename.sorted 
#
# David Kotz - April 2016, 2017

# for each input line, 
{
    # print the word
    printf "%s ", $1 | "sort";

    # accumulate the pairs in an array
    maxID = 0;
    for (i = 2; i < NF; i += 2) {
	id = $i;
	count[id] = $(i+1);
	maxID = id > maxID ? id : maxID;
    }

    # print the pairs in order
    for (id = 1; id <= maxID; id++)
	if (count[id] > 0)
	    printf "%d %d ", id, count[id] | "sort";
    print "" | "sort";

    # remove the array, ready for next line
    delete count;
}

END {
}

