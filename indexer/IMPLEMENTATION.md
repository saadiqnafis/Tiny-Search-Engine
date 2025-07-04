Overview
The indexer module in indexer.c is responsible for building and saving an inverted index from files generated by a crawler. The inverted index maps each unique word to a list of document IDs and the frequency with which each word appears in those documents. This enables efficient search and retrieval of relevant documents based on word queries.

Implementation Specification
Data Structures
index_t: The primary structure for the index, containing:

A hashtable_t to store words as keys.
A counters_t for each word as values, tracking document IDs and their counts within each document.
hashtable_t: A hash table for efficient lookup, where each entry’s key is a word, and the associated value is a counters_t structure with the document counts.

counters_t: A structure that keeps a word’s frequency data as (docID, count) pairs for all documents containing the word.

Key Functions
index_new
Prototype: index_t* index_new(const int num_slots);
Description: Allocates and initializes a new index_t structure with a specified number of slots in its hashtable_t. Returns a pointer to the new index or NULL if memory allocation fails.
index_insert
Prototype: void index_insert(index_t* index, const char* word, const int docID);
Description: Adds a word entry to the index, associating it with the specified docID. If the word is already in the index, increments the count for that docID. If it’s a new word, creates a counters_t for it and initializes the (docID, count) pair.
index_save
Prototype: bool index_save(index_t* index, const char* filename);
Description: Saves the index contents to a file, writing each word and its (docID, count) pairs. Each line in the file is in the format word docID count [docID count]....
index_load
Prototype: index_t* index_load(const char* filename);
Description: Reads an index file to rebuild an index_t structure in memory. Each line in the file represents a word with its associated (docID, count) pairs, which are added to a counters_t structure.
index_delete
Prototype: void index_delete(index_t* index);
Description: Frees all memory associated with the index and its structures.
File Format Specification
The index file has the following format:

Each line corresponds to a unique word and its document-specific data.
Format: word docID count [docID count]..., where each word is followed by one or more (docID, count) pairs.
Words are lowercase; docID and count are positive integers.
Words and (docID, count) pairs can be in any order.
Testing Plan
Unit Testing
Testing each function in isolation verifies correctness and robustness:

index_new: Confirm that it returns a valid, non-null pointer and correctly allocates memory.
index_insert:
Insert a new word and check if it appears in the index with the correct (docID, count) pair.
Insert the same word with the same docID multiple times and verify that count increments.
Insert the same word with different docID values and check if each docID is tracked separately.
index_save:
Check if the file is created and formatted as specified.
Save an empty index to ensure it creates an empty file.
Verify saving an index with multiple words and docID counts, ensuring correct format.
index_load:
Load a saved index file and check if the structure matches the original index.
Test loading files with multiple words and docID counts, verifying the data structure’s integrity.
index_delete: Verify that all memory associated with the index is correctly freed, checked using valgrind to ensure no memory leaks.
Integration Testing
Indexing from pageDirectory:

Use indexer on a pageDirectory generated by the crawler.
Save the resulting index and inspect the output file for correctness in format and data.
Load the saved index with index_load and verify it matches the original data structure.
Consistency Check:

Build an index from pageDirectory, save it as oldIndexFilename, then load and resave it as newIndexFilename. Confirm the contents of oldIndexFilename and newIndexFilename are identical.
Testing Script (testing.sh)
The testing script automates testing for valid and invalid arguments and runs valgrind to check for memory issues.

Automated Test Cases
Argument Validation:

Run indexer with no arguments, one argument, and more than two arguments to ensure proper error handling.
Test with an invalid or nonexistent pageDirectory.
Test with a read-only or invalid indexFilename.
Indexing with Various pageDirectory Inputs:

Run indexer on multiple valid pageDirectory directories to ensure it builds an index as expected.
Verify the index output by checking format and data accuracy.
Memory Checks:

Use valgrind on both indexer and indextest to verify there are no memory leaks or errors during execution.