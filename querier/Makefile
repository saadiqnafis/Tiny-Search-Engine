# Makefile for 'querier' module
#
# Nafis Saadiq Bhuiyan, October 2024
#

OBJS = querier.o 
LIBS = ../common/common.a ../libcs50/libcs50-given.a 

CFLAGS = -Wall -pedantic -std=c11 -ggdb -I../lib
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all
all: querier 
# target to create the executable
querier: querier.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# dependencies

# indextest: indextest.o
# 	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

querier.o: querier.c 


# Basic Test
test: querier
	bash testing.sh

# clean the directory of object files and executable

clean:
	rm -rf *.dsYM # MacOS debugger info
	rm -f *~ *.o
	rm -f querier
	rm -f core

