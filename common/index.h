/**
 * index - CS50 Index Module
 * 
 * Nafis Saadiq Bhuiyan, October 2024
 */

#ifndef __INDEX_H
#define __INDEX_H

#include <stdbool.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/mem.h"
#include "../libcs50/file.h"

typedef struct index{
    hashtable_t* table;
 }
 index_t;

index_t* index_new(const int num_slots);
void index_insert(index_t* index, const char* word, const int docID);
bool index_save(index_t* index, const char* filename);
index_t* index_load(const char* filename);
void index_delete(index_t* index);

#endif