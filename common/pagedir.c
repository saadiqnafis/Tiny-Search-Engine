/**
 * pagedir - CS50 Pagedir module
 * 
 *We create a re-usable module pagedir.c to handles the pagesaver mentioned in the design (writing a page to the pageDirectory), and marking it as a Crawler-produced pageDirectory (as required in the spec). We chose to write this as a separate module, in ../common, to encapsulate all the knowledge about how to initialize and validate a pageDirectory, and how to write and read page files, in one place... anticipating future use by the Indexer and Querier.
 * 
 * Nafis Saadiq Bhuiyan, October 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "../libcs50/webpage.h"
#include "../libcs50/mem.h"
#include "pagedir.h"
#include "../libcs50/file.h"

// Initializes the page directory by creating a .crawler file in it.
bool pagedir_init(const char* pageDirectory) {
    if (pageDirectory == NULL) {
        return false;
    }

    // Construct the path for the .crawler file
    char crawlerFilePath[1000];
    snprintf(crawlerFilePath, sizeof(crawlerFilePath), "%s/.crawler", pageDirectory);

    // Create the .crawler file to mark the directory
    FILE* fp = fopen(crawlerFilePath, "w");
    if (fp == NULL) {
        return false;
    }
    fclose(fp);
    return true;
}

// Saves a webpage to the specified directory with a given document ID
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID) {
    if (page == NULL || pageDirectory == NULL || docID < 1) {
        return;
    }

    // Construct the path for the page file
    char pageFilePath[1000];
    snprintf(pageFilePath, sizeof(pageFilePath), "%s/%d", pageDirectory, docID);

    // Open the file and write the contents
    FILE* fp = fopen(pageFilePath, "w");
    if (fp != NULL) {
        fprintf(fp, "%s\n%d\n%s", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));
        fclose(fp);
    }
}

// Validates if a directory is a crawler-produced directory by checking for the .crawler file
bool pagedir_validate(const char* pageDirectory) {
    if (pageDirectory == NULL) {
        return false;
    }

    // Construct the path to the .crawler file
    char crawlerFilePath[1000];
    snprintf(crawlerFilePath, sizeof(crawlerFilePath), "%s/.crawler", pageDirectory);

    // Check if the .crawler file exists
    struct stat statbuf;
    return (stat(crawlerFilePath, &statbuf) == 0);
}

// Loads a webpage from a file in the specified directory and document ID
webpage_t* pagedir_load(const char* pageDirectory, const int docID) {
    if (pageDirectory == NULL || docID < 1) {
        return NULL;
    }

    // Construct the path for the page file
    char pageFilePath[1000];
    snprintf(pageFilePath, sizeof(pageFilePath), "%s/%d", pageDirectory, docID);

    // Open the file
    FILE* fp = fopen(pageFilePath, "r");
    if (fp == NULL) {
        return NULL;
    }

    // Read the URL
    char* url = file_readLine(fp);
    if (url == NULL) {
        fclose(fp);
        return NULL;
    }

    // Read the depth
    int depth;
    if (fscanf(fp, "%d\n", &depth) != 1) {
        mem_free(url);
        fclose(fp);
        return NULL;
    }

    // Read the HTML content
    char* html = file_readFile(fp);
    fclose(fp);
    if (html == NULL) {
        mem_free(url);
        return NULL;
    }

    // Create the webpage
    webpage_t* page = webpage_new(url, depth, html);
    if (page == NULL) {
        mem_free(url);
        mem_free(html);
    }
    return page;
}
