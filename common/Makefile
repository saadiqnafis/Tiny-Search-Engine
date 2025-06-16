# Makefile for the common directory
# Builds and compiles utility functions into a library common.a
# Nafis Saadiq Bhuiyan, October 2024

# Object files and the target library
OBJS = pagedir.o index.o word.o
LIB = common.a 

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I../libcs50 -I../common
CC = gcc

# Build $(LIB) by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h
index.o: index.h
word.o: word.h

.PHONY: clean

# Clean up after compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o
	rm -f common.a
