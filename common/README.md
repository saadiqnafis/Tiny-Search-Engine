# CS50 Lab 4
## CS50 Fall 2024

### Common

For now, this directory comprises code for initializing the "pageDirectory" and saving webpages there. Moreover, it contains `index`, a module that to implement an abstract `index_t` type that represents an index in memory, and supports functions like `index_new()`, `index_delete()`, `index_save()`, and so forth. Apart from that, it also contains the `word` module which just contains a function to normalize the words.

### Usage
There's a `Makefile` that will, by default, build the `common.a` library. It also has a `clean` target that removes files produced by Make.

