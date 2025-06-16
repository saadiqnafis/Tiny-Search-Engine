/**
 * pagedir - CS50 Pagedir module
 * 
 *
 * 
 * Nafis Saadiq Bhuiyan, October 2024
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"

/* pagedir_init
 * This constructs the pathname for the .crawler file in that directory
open the file for writing; on error, return false.
close the file and return true.
 * */
bool pagedir_init(const char* pageDirectory);
/* pagedir_save
 * This constructs the pathname for the page file in pageDirectory
open that file for writing
print the URL
print the depth
print the contents of the webpage
close the file
 * */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

// Validates if a directory is a crawler-produced directory by checking for the .crawler file

bool pagedir_validate(const char* pageDirectory);

// Loads a webpage from a file in the specified directory and document ID
webpage_t* pagedir_load(const char* pageDirectory, const int docID);
#endif
