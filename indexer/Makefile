# Makefile for 'crawler' module
#
# Nafis Saadiq Bhuiyan, October 2024
#

OBJS = indexer.o indextest.o 
LIBS = ../common/common.a ../libcs50/libcs50-given.a 

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../lib
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all
all: indexer indextest
# target to create the executable
indexer: indexer.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
# dependencies

indextest: indextest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indexer.o: indexer.c 
indextest.o: indextest.c


# Basic Test
test: indexer
	bash testing.sh

# clean the directory of object files and executable

clean:
	rm -rf *.dsYM # MacOS debugger info
	rm -f *~ *.o
	rm -f indexer indextest
	rm -f core

