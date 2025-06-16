# CS50 Lab 6
## CS50 Fall 2024

### Querier

This directory has the files `querier.c` which is the implementation of our `querier` module. It has the `Makefile` that we can use to compile crawler and run the test. Moreover, we have our test file `testing.sh` and our output file `testing.out`.
#### Errors Noticed:
Unfortunately, when testing memory loss, we found that 
We have `make clean` tha removes files produced by Make or our tests.
We also have `make test` that tests our indexer.

We save the output of our tests using `make test >& testing.out`

