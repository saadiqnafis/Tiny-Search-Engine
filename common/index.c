/**
 * Index - CS50 Index module
 * 
 * The index module is a module to implement an abstract index_t type that represents an index in memory, and supports functions like index_new(), index_delete(), index_save(), and so forth. Much of it is a wrapper for a hashtable.
 * 
 * Nafis Saadiq Bhuiyan, October 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "index.h"

// Definition of index_t data structure
// struct index {
//     hashtable_t* table;
// };
// index_t;
// Helper function to free counters in the hashtable
void itemcount_delete(void* item) {
    if (item != NULL) {
        counters_delete(item);
    }
}

// Create a new index with a specified number of hashtable slots
index_t* index_new(const int num_slots) {
    index_t* index = mem_malloc(sizeof(index_t));
    if (index == NULL) {
        return NULL;
    }
    index->table = hashtable_new(num_slots);
    if (index->table == NULL) {
        mem_free(index);
        return NULL;
    }
    return index;
}

// Inserts a word with the document ID into the index, or increments the count if it already exists
void index_insert(index_t* index, const char* word, const int docID) {
    if (index == NULL || word == NULL || docID < 1) {
        return;
    }

    counters_t* counter = hashtable_find(index->table, word);
    if (counter == NULL) {
        counter = counters_new();
        hashtable_insert(index->table, word, counter);
    }
    counters_add(counter, docID);
}

// Helper function to save a word's counters to a file
void index_save_counter_helper(void* fp, const int docID, const int count) {
    if (count > 0) {
        fprintf(fp, " %d %d", docID, count);
    }
}

// Helper function to save each word and its counters to a file
void index_save_helper(void* fp, const char* word, void* item) {
    counters_t* counter = item;
    if (counter != NULL) {
        fprintf(fp, "%s", word);
        counters_iterate(counter, fp, index_save_counter_helper);
        fprintf(fp, "\n");
    }
}

// Save the index to a file
bool index_save(index_t* index, const char* filename) {
    if (index == NULL || filename == NULL) {
        return false;
    }

    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        return false;
    }

    hashtable_iterate(index->table, fp, index_save_helper);

    fclose(fp);
    return true;
}

// Load an index from a file
index_t* index_load(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        return NULL;
    }

    int numLines = file_numLines(fp);
    int num_slots = numLines > 0 ? numLines * 2 : 500;  // Estimate table size
    index_t* index = index_new(num_slots);
    if (index == NULL) {
        fclose(fp);
        return NULL;
    }

    char* word;
    while ((word = file_readWord(fp)) != NULL) {
        counters_t* counter = counters_new();
        int docID, count;

        while (fscanf(fp, "%d %d", &docID, &count) == 2) {
            counters_set(counter, docID, count);
        }

        hashtable_insert(index->table, word, counter);
        mem_free(word);
    }

    fclose(fp);
    return index;
}

// Delete the index and free memory
void index_delete(index_t* index) {
    if (index != NULL) {
        hashtable_delete(index->table, itemcount_delete);
        mem_free(index);
    }
}
