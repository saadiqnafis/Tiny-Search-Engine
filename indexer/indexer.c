/**
 * Indexer - CS50 Indexer module
 * 
 * The indexer is a standalone program that reads the document files produced by the crawler, builds an index, and writes that index to a file.
 * 
 * Nafis Saadiq Bhuiyan, October 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"

// Function prototypes
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
static void indexBuild(index_t* index, const char* pageDirectory);
static void indexPage(index_t* index, webpage_t* page, const int docID);


// Main function
int main(int argc, char* argv[])
{
    char* pageDirectory;
    char* indexFilename;

    // Parse the command-line arguments and validate them
    parseArgs(argc, argv, &pageDirectory, &indexFilename);

    // Create a new index object
    index_t* index = index_new(900);
    if (index == NULL) {
        fprintf(stderr, "Error: Could not create index\n");
        exit(1);
    }

    // Build the index from the pages in pageDirectory
    indexBuild(index, pageDirectory);

    // Save the index to the index file
    if (!index_save(index, indexFilename)) {
        fprintf(stderr, "Error: Could not save index to file\n");
        index_delete(index);
        exit(2);
    }

    // Clean up
    index_delete(index);
    
    return 0;
}

// Parses command-line arguments, validates them, and assigns values to variables
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: ./indexer pageDirectory indexFilename\n");
        exit(1);
    }

    // Validate pageDirectory
    *pageDirectory = argv[1];
    if (!pagedir_validate(*pageDirectory)) {
        fprintf(stderr, "Error: Invalid pageDirectory\n");
        exit(2);
    }

    // Validate indexFilename
    *indexFilename = argv[2];
    FILE* fp = fopen(*indexFilename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not write to indexFilename\n");
        exit(3);
    }
    fclose(fp);
}

// Builds an index from the documents in pageDirectory
static void indexBuild(index_t* index, const char* pageDirectory)
{
    int docID = 1;
    webpage_t* page;

    // Loop through each document in the directory by document ID
    while ((page = pagedir_load(pageDirectory, docID)) != NULL) {
        indexPage(index, page, docID);
        webpage_delete(page);
        docID++;
    }
}

// Indexes a page by scanning for words and adding to the index
static void indexPage(index_t* index, webpage_t* page, const int docID)
{
    int pos = 0;
    char* word;

    // Get each word from the page
    while ((word = webpage_getNextWord(page, &pos)) != NULL) {
        if (strlen(word) >= 3) {
            to_lowercase(word);
            index_insert(index, word, docID);
        }
        mem_free(word);
    }
}


