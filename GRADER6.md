# Lab 6 rubric

## Student: Jacob Werzinsky
## Grader: Sylvester Elorm Coch

## Total:98/100

## (10) Delivery
  * (3) Proper git tag `lab6submit`.
  * (2) git commit history - logical groupings of changes in each commit, and meaningful commit messages.
  * (3) Proper file structure: tse directory and subdirectories `querier` and `common` exist, and `querier` must include appropriate `.gitignore`.
  * (2) no executable files, object files, core dump files, editor backup files, or extraneous files existing in any directory.


## (12) Makefiles
  * (0) `tse/Makefile` should now build all subdirectories.
  * (4) `querier/Makefile`
	  * (1) exists
	  * (1) has proper target to build `querier `
	  * (0) optionally has target to build `fuzzquery`
	  * (1) uses standard CFLAGS (or with `c11` changed to `gnu11`).
	  * (1) has an appropriate `clean` rule.
  * (4) directory `common/` exists and `common/Makefile`
	  * (1) exists
	  * (1) has proper targets to build `common.a`
	  * (1) uses CS50 standard CFLAGS.
	  * (1) has an appropriate `clean` rule.
  * (4) Builds `querier` (and optionally `fuzzquery`) with no compilation errors or warnings *when compiled from the top-level directory*. 

## (12) Documentation
In `tse/querier/`,
  * (2) `README.md`, which says how to build `querier`.
  * (4) `DESIGN.md`, which describes the abstract design of the querier with no language-specific details.
  * (4) `IMPLEMENTATION.md`: describes the implementation of `querier`.
  * (2) `testing.sh`, which may refer to testing results included in a separate file (e.g., testing.out, testing.txt)

## (6) Testing
  * (5) `tse/querier/testing.sh` should have good test cases and good comments
  * (1) `tse/querier/testing.out` should be the output of `make test &> testing.out`


## (20) Coding Style
  * Their querier-specific code should be isolated from the data structure code.
  * Their querier code should not 'peek' inside opaque types.
  * They *should* have a clean, well-organized code for parsing and validating command-line args that is not interwoven with the overall program flow & logic.
  * The code should be rigorous in checking for memory-allocation errors or file I/O methods. 
  * Well-designed code structure
    * avoid deep nested loops or conditional blocks.
    * consistent indentation and spacing.
    * sufficient comments.

## (29/30) Functionality
  * (-1) Cannot handle empty and blank queries. 

## (9/10) Memory leaks, according to valgrind
  * (3/4) free of *memory errors*. Leaks memory on empty queries (queries with just spaces).
  * (4) free of *lost memory blocks*.
  * (2) free of *still reachable* memory blocks.


