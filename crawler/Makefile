# Makefile for 'crawler' module
#
# Nafis Saadiq Bhuiyan, October 2024
#

OBJS = crawler.o
LIBS = ../libcs50/libcs50-given.a ../common/common.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# target to create the executable
crawler: crawler.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
# dependencies
crawler.o: crawler.c 

.PHONY: test clean

# Basic Test
test: crawler
	bash testing.sh


# clean the directory of object files and executable

clean:
	rm -rf *.dsYM # MacOS debugger info
	rm -f *~ *.o
	rm -f crawler
	rm -f core

