/**
 * crawler.c - CS50 Crawler module
 * 
 * Implements a crawler that crawls webpages
 * 
 * Nafis Saadiq Bhuiyan, October 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <stdbool.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"

// Function prototypes
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

int main(const int argc, char* argv[]) {
    char* seedURL = NULL;
    char* pageDirectory = NULL;
    int maxDepth = 0;

    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    crawl(seedURL, pageDirectory, maxDepth);

    return 0;
}

// Parses the command-line arguments, normalizes the seedURL, and validates parameters
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth) {
    if (argc != 4) {
        fprintf(stderr, "Error: Invalid number of arguments.\n");
        exit(1);
    }

    // Validate seedURL
    *seedURL = normalizeURL(argv[1]);
    if (!isInternalURL(*seedURL)) {
        fprintf(stderr, "Error: Seed URL is not internal.\n");
        exit(2);
    }

    // Validate pageDirectory
    *pageDirectory = argv[2];
    if (!pagedir_init(*pageDirectory)) {
        fprintf(stderr, "Error: Unable to initialize page directory.\n");
        exit(3);
    }

    // Validate maxDepth
    *maxDepth = atoi(argv[3]);
    if (*maxDepth < 0 || *maxDepth > 10) {
        fprintf(stderr, "Error: Invalid maxDepth value.\n");
        exit(4);
    }
}

// The core crawl function that fetches, scans, and saves webpages
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth) {
    // Initialize the hashtable (with 200 slots) and bag for crawling
    hashtable_t* pagesSeen = hashtable_new(200);
    bag_t* pagesToCrawl = bag_new();

    // Insert seedURL into hashtable and bag
    hashtable_insert(pagesSeen, seedURL, "");
    webpage_t* seedPage = webpage_new(seedURL, 0, NULL);
    bag_insert(pagesToCrawl, seedPage);

    int documentID = 1;

    // Loop over pages to crawl
    webpage_t* page;
    while ((page = bag_extract(pagesToCrawl)) != NULL) {
        // Pause for at least 1 second between page fetches
        sleep(1);

        // Fetch the page HTML content
        if (webpage_fetch(page)) {
            // Save the page to the pageDirectory
            pagedir_save(page, pageDirectory, documentID++);
            
            // If the current page depth is less than maxDepth, scan for links
            if (webpage_getDepth(page) < maxDepth) {
                pageScan(page, pagesToCrawl, pagesSeen);
            }
        }

        // Delete the webpage after processing
        webpage_delete(page);
    }

    // Cleanup
    hashtable_delete(pagesSeen, NULL);
    bag_delete(pagesToCrawl, NULL);
}

// Scans a webpage for internal links and adds them to the bag and hashtable
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen) {
    int pos = 0;
    char* nextURL;

    while ((nextURL = webpage_getNextURL(page, &pos)) != NULL) {
        char* normalizedURL = normalizeURL(nextURL);
        
        if (isInternalURL(normalizedURL)) {
            // Try to insert the URL into the hashtable
            if (hashtable_insert(pagesSeen, normalizedURL, "")) {
                // If successful, create a new webpage and add it to the bag
                webpage_t* newPage = webpage_new(normalizedURL, webpage_getDepth(page) + 1, NULL);
                bag_insert(pagesToCrawl, newPage);
            }
        }
        
        free(nextURL);  // Free the memory allocated by webpage_getNextURL
    }
}
