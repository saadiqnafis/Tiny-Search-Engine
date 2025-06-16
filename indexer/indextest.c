/*
 * indextest.c - test program for CS50 index module
 *
 * This program is a basic unit test for the index module.
 * 
 * Execution:
    execute from a command line with usage syntax
    ./indextest oldIndexFilename newIndexFilename
    where oldIndexFilename is the name of a file produced by the indexer, and
    where newIndexFilename is the name of a file into which the index should be written.
    For example: 
        ./indextest ../crawler/data/letters-2.index ../crawler/data/letters-2-read.index
 * It tests the key functions: index_load, index_save, index_delete
 *
 * CS50, Nafis Saadiq Bhuiyan, October 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../common/index.h"


int main(int argc, char* argv[]) {
    // Ensure exactly two arguments are provided
    if (argc != 3) {
        fprintf(stderr, "Usage: ./indextest oldIndexFilename newIndexFilename\n");
        return 1;
    }

    // Retrieve the filenames from command-line arguments
    char* oldIndexFilename = argv[1];
    char* newIndexFilename = argv[2];

    // Load the index from the old index file
    index_t* index = index_load(oldIndexFilename);
    if (index == NULL) {
        fprintf(stderr, "Error: could not load index from file %s\n", oldIndexFilename);
        return 2;
    }

    // Save the index to the new index file
    if (!index_save(index, newIndexFilename)) {
        fprintf(stderr, "Error: could not save index to file %s\n", newIndexFilename);
        index_delete(index);
        return 3;
    }

    // Clean up
    index_delete(index);
    printf("Index successfully loaded from %s and saved to %s\n", oldIndexFilename, newIndexFilename);
    return 0;
}
