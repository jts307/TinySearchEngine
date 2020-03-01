## Lab 6 Test Cases

<mark>you can find the solution's executable under ~cs50/demo/

To use it, first log into a CS server, then run
```bash
$ ~cs50/demo/querier [pagedirectory] [indexfilename]
```

### Small test cases (12 = 1 * 12, 1 point for each)
#### T1: check # of arguments
    ./querier ~cs50/data/tse-output/letters-depth-6/ ~cs50/data/tse-output/letters-index-6 data

##### Expected output: 
    usage: ./querier: pageDirectory indexFilename

#### T2: check crawler directory
    ./querier ../common ~cs50/data/tse-output/letters-index-6

##### Expected output:
    usage: ./querier: '../common' is not a crawler data directory

#### T3: check existence of index file
    ./querier ~cs50/data/tse-output/letters-depth-6/ ../index

##### Expected output:
    index: cannot open index file '../index'
    ./querier: cannot load index from '../index'

### Syntax errors in query input (1 point each)
Run the querier with any folder names, e.g

    ./querier ~cs50/data/tse-output/letters-depth-6/ ~cs50/data/tse-output/letters-index-6

#### T4
    and Dartmouth
##### Expected output: 
    Error, and cannot be the first

#### T5
    or Dartmouth
##### Expected output: 
    Error, or cannot be the first

#### T6
    Dartmouth or 
##### Expected output: 
    Error, or cannot be the last

#### T7
    Dartmouth and
##### Expected output: 
    Error, and cannot be the last

#### T8
    MIT and or Dartmouth
##### Expected output: 
    Error, and and or cannot be adjacent

#### T9
    Boston or and Hanover
##### Expected output: 
    Error, and and or cannot be adjacent

#### T10
    computer science 60
##### Expected output: 
    bad char 6 in query

#### T11
    I love computer science!
##### Expected output: 
    bad char ! in query

#### T12
    Tiny Search-engine
##### Expected output: 
    bad char - in query


### Big test cases (18 = 2 * 9, 2 points each, focus on ranking and whether the result is complete)
#### T13-T21
    ./querier ~cs50/data/tse-output/toscrape-depth-1/ ~cs50/data/tse-output/toscrape-index-1

#### T13
    years
##### Expected output:
    Query: years 
    Matches 7 documents (ranked):
    score   4 doc  18: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/sapiens-a-brief-history-of-humankind_996/index.html
    score   3 doc  15: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/the-coming-woman-a-novel-based-on-the-life-of-the-infamous-feminist-victoria-woodhull_993/index.html
    score   2 doc   9: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/scott-pilgrims-precious-little-life-scott-pilgrim-1_987/index.html
    score   2 doc  16: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/the-dirty-little-secrets-of-getting-your-dream-job_994/index.html
    score   2 doc  19: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/sharp-objects_997/index.html
    score   1 doc   5: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/mesaerion-the-best-science-fiction-stories-1800-1849_983/index.html
    score   1 doc  13: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/the-black-maria_991/index.html

#### T14
    book or story
##### Expected output:
    Query: book or story 
    Matches 16 documents (ranked):
    score   9 doc  14: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/the-boys-in-the-boat-nine-americans-and-their-epic-quest-for-gold-at-the-1936-berlin-olympics_992/index.html
    score   4 doc  15: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/the-coming-woman-a-novel-based-on-the-life-of-the-infamous-feminist-victoria-woodhull_993/index.html
    score   3 doc   6: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/olio_984/index.html
    score   3 doc   5: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/mesaerion-the-best-science-fiction-stories-1800-1849_983/index.html
    score   2 doc  11: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/shakespeares-sonnets_989/index.html
    score   2 doc  21: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/tipping-the-velvet_999/index.html
    score   2 doc  49: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/art_25/index.html
    score   2 doc   7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/our-band-could-be-your-life-scenes-from-the-american-indie-underground-1981-1991_985/index.html
    score   1 doc  16: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/the-dirty-little-secrets-of-getting-your-dream-job_994/index.html
    score   1 doc  18: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/sapiens-a-brief-history-of-humankind_996/index.html
    score   1 doc  57: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/sports-and-games_17/index.html
    score   1 doc  62: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/religion_12/index.html
    score   1 doc   2: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/page-2.html
    score   1 doc  19: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/sharp-objects_997/index.html
    score   1 doc  63: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/childrens_11/index.html
    score   1 doc  68: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/classics_6/index.html

#### T15
    book and history
##### Expected output:
    Query: book and history 
    Matches 7 documents (ranked):
    score   2 doc  15: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/the-coming-woman-a-novel-based-on-the-life-of-the-infamous-feminist-victoria-woodhull_993/index.html
    score   1 doc  14: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/the-boys-in-the-boat-nine-americans-and-their-epic-quest-for-gold-at-the-1936-berlin-olympics_992/index.html
    score   1 doc  16: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/the-dirty-little-secrets-of-getting-your-dream-job_994/index.html
    score   1 doc  18: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/sapiens-a-brief-history-of-humankind_996/index.html
    score   1 doc  49: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/art_25/index.html
    score   1 doc  57: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/sports-and-games_17/index.html
    score   1 doc  62: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/religion_12/index.html

#### T16
    art price
##### Expected output:
    Query: art price 
    Matches 2 documents (ranked):
    score   2 doc  13: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/the-black-maria_991/index.html
    score   1 doc   9: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/scott-pilgrims-precious-little-life-scott-pilgrim-1_987/index.htmlindex.html

#### T17
    music band or painting 
##### Expected output:
    Query: music band or painting 
    Matches 5 documents (ranked):
    score   3 doc   7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/our-band-could-be-your-life-scenes-from-the-american-indie-underground-1981-1991_985/index.html
    score   1 doc   1: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html
    score   1 doc  60: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/music_14/index.html
    score   1 doc  70: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/historical-fiction_4/index.html
    score   1 doc  73: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books_1/index.html


#### T18
    modern music or classical poem
##### Expected output:
    Query: modern music or classical poem 
    Matches 1 documents (ranked):
    score   1 doc  56: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/add-a-comment_18/index.html

#### T19
    political nonfiction or modern art or humor story 
##### Expected output:
    Query: political nonfiction or modern art or humor story 
    Matches 6 documents (ranked):
    score   1 doc  42: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/history_32/index.html
    score   1 doc  56: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/add-a-comment_18/index.html
    score   1 doc   2: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/page-2.html
    score   1 doc  49: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/art_25/index.html
    score   1 doc  63: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/childrens_11/index.html
    score   1 doc  68: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/classics_6/index.html

#### T20
    eat fat get thin or the time keeper or the black maria 
##### Expected output:
    Query: eat fat get thin or the time keeper or the black maria 
    Matches 12 documents (ranked):
    score   6 doc  13: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/the-black-maria_991/index.html
    score   1 doc  27: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/health_47/index.html
    score   1 doc   1: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html
    score   1 doc   7: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/our-band-could-be-your-life-scenes-from-the-american-indie-underground-1981-1991_985/index.html
    score   1 doc   8: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/rip-it-up-and-start-again_986/index.html
    score   1 doc   9: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/scott-pilgrims-precious-little-life-scott-pilgrim-1_987/index.html
    score   1 doc  10: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/set-me-free_988/index.html
    score   1 doc  11: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/shakespeares-sonnets_989/index.html
    score   1 doc  12: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/starving-hearts-triangular-trade-trilogy-1_990/index.html
    score   1 doc  51: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/poetry_23/index.html
    score   1 doc  64: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/fiction_10/index.html
    score   1 doc  73: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books_1/index.html

#### T21
    paris kitchen recipes baking or white cat bear whale or best reviews 
##### Expected output:
    Query: paris kitchen recipes baking or white cat bear whale or best novels 
    Matches 6 documents (ranked):
    score   1 doc  41: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/food-and-drink_33/index.html
    score   1 doc  63: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/childrens_11/index.html
    score   1 doc   1: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html
    score   1 doc   4: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/libertarianism-for-beginners_982/index.html
    score   1 doc  58: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books/science-fiction_16/index.html
    score   1 doc  73: http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/catalogue/category/books_1/index.html






