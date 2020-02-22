# Grading: Lab 4, Jacob Werzinsky
## Total: 89/100

Raw score: 99/100

-10 points for one day late.

### Delivery: 19/20

* -1 for having data files produced by crawler in your repository.
From the assignment page: "You shall NOT commit any data files produced by the crawler..."
* In your crawler Makefile, you should list `crawler` as a dependency. As it is, `make test` doesn't work after having run `make clean` because make doesn't know to build crawler.

### Documentation & Testing: 22/22

### Code Quality: 20/20

### Functionality: 38/38

* While your code appears to be functional, in the future, you should print progress so that a user can see how the program is progressing. For instance, print every time a new page is explored.
Without doing this, it's hard to tell how fast your crawler is progressing.
