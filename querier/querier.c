/*

CS50 tiny search engine - querier.c

This code is the querier part of the tiny search engine, where an index is read in
and it reads the queary (which is an implicit or explicit and/or combination) and returns
all of the webpages that match the queary

Nafis Saadiq Bhuiyan, November 2024

to run: ./querier crawlerDirectory indexFile

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include "../libcs50/mem.h"
#include "../libcs50/file.h"
#include "../common/index.h"
#include "../common/pagedir.h"
#include "../common/word.h"

/**************** local types ****************/
// allow pass in 2 counters into iterate to compare and combine
typedef struct compareCounters {
counters_t* output;
counters_t* compared;
} compareCounters_t;

// keeps track of all the documents and their scores
// stores highest ranking document and its score
typedef struct {
counters_t* counters;
int maxScore;
int maxDocID;
} maxScore_t;

// stores urls with their score
typedef struct {
const char* pageDirectory;
int score;
} docData_t;

/**************** function prototypes ****************/
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename);
static void processQueries(index_t* index, const char* pageDirectory);
static bool validateQuery(const char* query);
static void executeQuery(const char* query, index_t* index, const char* pageDirectory);
static counters_t* processAndSequence(char** words, int* pos, const int numWords, index_t* index);
static void intersectCounters(counters_t* output, counters_t* compared);
static void unionCounters(counters_t* output, counters_t* compared);
static void intersectHelper(void* arg, const int key, const int count);
static void unionHelper(void* arg, const int key, const int count);
static void printDocumentRanking(counters_t* results, const char* pageDirectory);
static void updateMaxScore(void* arg, const int key, const int count);
static int counters_count(counters_t* ctrs);

/**************** main ****************/
// method to run query
int main(const int argc, char* argv[])
{
// Read in command line arguments
char* pageDirectory = NULL;
char* indexFilename = NULL;
parseArgs(argc, argv, &pageDirectory, &indexFilename);
// Load index
// FILE* fp = fopen(indexFilename, "r");
// if (fp == NULL) {
// fprintf(stderr, "Cannot open index file '%s'\n", indexFilename);
// exit(1);
// }
// create index data structure to store index file information
index_t* index = index_new(500);
if (!index_load(indexFilename)) {
fprintf(stderr, "Failed to load index from '%s'\n", indexFilename);
// fclose(fp);
index_delete(index);
exit(1);
}
else {
    index = index_load(indexFilename);
}
// fclose(fp);
// Process queries
processQueries(index, pageDirectory);
index_delete(index);
return 0;
}

/**************** parseArgs ****************/
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFilename)
// the purpose of this function is to read in the command line arguments and make them usable
{
// check if user gave the correct number of arguments
if (argc != 3) {
fprintf(stderr, "Usage: %s pageDirectory indexFilename\n", argv[0]);
exit(1);
}
// assign pointers
*pageDirectory = argv[1];
*indexFilename = argv[2];

// check if crawler directory is a crawler directory
char crawlerFilePath[999];
snprintf(crawlerFilePath, sizeof(crawlerFilePath), "%s/.crawler", *pageDirectory);
FILE* crawlerFile = fopen(crawlerFilePath, "r");
if (crawlerFile == NULL) {
fprintf(stderr, "Error: Directory %s is not a crawler directory (missing .crawler file)\n", *pageDirectory);
exit(2);
}
fclose(crawlerFile);

// check if can read index
FILE* file = fopen(*indexFilename, "r");
if (file == NULL){
fprintf(stderr, "Error: can't read index file %s\n", *indexFilename);
exit(3);
}
fclose(file);
}

/**************** processQueries ****************/
static void
processQueries(index_t* index, const char* pageDirectory)
// this method is here to process the user inputted queries from stdin
{
char* line;
printf("Query? \n");
// read in lines from stdin
while ((line = file_readLine(stdin)) != NULL) {
if (strlen(line) == 0) {
printf("Query: ");
free(line);
continue;
}
// Convert to lowercase
char* lowerLine = to_lowercase(line);
free(line);
// Remove extra spaces
char* cleaned = mem_malloc(strlen(lowerLine) + 1);
int j = 0;
int space = 1;
for (int i = 0; lowerLine[i] != '\0'; i++) {
if (isspace(lowerLine[i])) {
if (!space) {
cleaned[j++] = ' ';
space = 1;
}
} else {
cleaned[j++] = lowerLine[i];
space = 0;
}
}
// Remove trailing space if exists
if (j > 0 && cleaned[j-1] == ' ') {
j--;
}
cleaned[j] = '\0';
free(lowerLine);
lowerLine = cleaned;

// check if valid entry per requirements spec
if (validateQuery(lowerLine)) {
printf("Query: %s\n", lowerLine);
executeQuery(lowerLine, index, pageDirectory);
}
// get ready for next line
free(lowerLine);
printf("------------------------------------------------------------\n");
printf("Query? ");
}
printf("\n");
}

/**************** validateQuery ****************/
// this method is to check if the queary is a valid queary and
// doesn't break the rules defined in teh spec
static bool
validateQuery(const char* query)
{
// check if query is empty
if (query == NULL){
return false;
}
// Check for invalid characters
for (const char* p = query; *p != '\0'; p++) {
if (!isalpha(*p) && !isspace(*p)) {
fprintf(stderr, "Error: bad character '%c' in query\n", *p);
return false;
}
}
// Split into words
char* queryCopy = mem_malloc(strlen(query) + 1);
strcpy(queryCopy, query);
char* words[999];
int numWords = 0;
char* word = strtok(queryCopy, " \t\n");
while (word != NULL && numWords < 999) {
words[numWords++] = word;
word = strtok(NULL, " \t\n");
}
// Check for starting/ending with and/or
if (numWords > 0) {
if (strcmp(words[0], "and") == 0 || strcmp(words[0], "or") == 0) {
fprintf(stderr, "Error: '%s' cannot be first\n", words[0]);
free(queryCopy);
return false;
}
if (strcmp(words[numWords-1], "and") == 0 ||
strcmp(words[numWords-1], "or") == 0) {
fprintf(stderr, "Error: '%s' cannot be last\n", words[numWords-1]);
free(queryCopy);
return false;
}
}
// Check is and/or are next to eachother
for (int i = 0; i < numWords - 1; i++) {
if ((strcmp(words[i], "and") == 0 || strcmp(words[i], "or") == 0) &&
(strcmp(words[i+1], "and") == 0 || strcmp(words[i+1], "or") == 0)) {
fprintf(stderr, "Error: 'or' and 'and' cannot be adjacent\n");
free(queryCopy);
return false;
}
}
free(queryCopy);
return true;
}

/**************** executeQuery ****************/
static void
executeQuery(const char* query, index_t* index, const char* pageDirectory)
// this method takes in the users query and the index, and returns all matches to the queary
// from the page directory
{
// Split query into words
char* queryCopy = mem_malloc(strlen(query) + 1);
strcpy(queryCopy, query);
char* words[100];
int numWords = 0;
char* word = strtok(queryCopy, " \t\n");
while (word != NULL && numWords < 100) {
words[numWords++] = word;
word = strtok(NULL, " \t\n");
}
if (numWords == 0) {
free(queryCopy);
return;
}
counters_t* result = counters_new();
int pos = 0;
// Process first and-sequence
counters_t* andResult = processAndSequence(words, &pos, numWords, index);
if (andResult == NULL) {
    printf("Andresult is null");
}
if (andResult != NULL) {
unionCounters(result, andResult);
counters_delete(andResult);
}
// Pro cess remaining or-separated and-sequences, wher assume and is default between words
while (pos < numWords) {
if (strcmp(words[pos], "or") == 0) {
pos++;
andResult = processAndSequence(words, &pos, numWords, index);
if (andResult != NULL) {
unionCounters(result, andResult);
counters_delete(andResult);
}
} else {
pos++;
}
}
// Print results
if (counters_count(result) == 0) {
printf("No documents match.\n");
} else {
printDocumentRanking(result, pageDirectory);
}
counters_delete(result);
free(queryCopy);
}

/**************** processAndSequence ****************/
// this method processed all 'and's in the query or words that are next to eachother
// processed words until hits "or" or the end of the query
static counters_t*
processAndSequence(char** words, int* pos, const int numWords, index_t* index)
{
if (*pos >= numWords){
return NULL;
}
// Get first word's counters
counters_t* result = NULL;

// checks words until the end of the line or if sees the word 'or'
while (*pos < numWords && strcmp(words[*pos], "or") != 0) {
// ignore all 'and's as we treat all space like an and
if (strcmp(words[*pos], "and") != 0) {
// grab counter from word in index
counters_t* wordCounters = hashtable_find(index->table, words[*pos]);
if (wordCounters != NULL) {
// first time reading in make the counter
if (result == NULL) {
result = counters_new();
counters_iterate(wordCounters, result, (void (*)(void*, const int, const int))counters_set);
// if read in word before then find the intersection of the counters to simulate an 'and'
} else {
intersectCounters(result, wordCounters);
}
} else {
// If any word isn't found, the entire AND sequence should return no results
if (result != NULL) {
counters_delete(result);
}
// update result to show no matches
result = counters_new();
break;
}
}
(*pos)++;
}
return result;
}

/**************** intersectCounters ****************/
// finds the intersection of counters to simulate an 'and'
static void
intersectCounters(counters_t* output, counters_t* compared)
{
if (output == NULL || compared == NULL){
return;
}
compareCounters_t args = {output, compared};
counters_iterate(output, &args, intersectHelper);
}

/**************** unionCounters ****************/
// method to find the union of counters to simulate an or
static void
unionCounters(counters_t* output, counters_t* compared)
{
if (output == NULL || compared == NULL){
return;
}
counters_iterate(compared, output, unionHelper);
}

/**************** intersectHelper ****************/
// helper method to deal with ands between counters
static void
intersectHelper(void* arg, const int key, const int count)
{
compareCounters_t* compare = arg;
int comparedCount = counters_get(compare->compared, key);
if (comparedCount == 0) {
counters_set(compare->output, key, 0);
// for and need to do the lesser of the appearances as needs to be the 'and's
} else {
int smallerCount;
if (count < comparedCount) {
smallerCount = count;
} else {
smallerCount = comparedCount;
}

// Store this smaller count in our output counter for this document
counters_set(compare->output, key, smallerCount);
}
}

/**************** unionHelper ****************/
// helper method to deal with ors between counters

static void
unionHelper(void* arg, const int key, const int count)
{
counters_t* output = arg;
int currentCount = counters_get(output, key);
counters_set(output, key, currentCount + count);
}

/**************** printDocumentRanking ****************/
// method to calculate the document rankings and print them in numerical order

static void
printDocumentRanking(counters_t* results, const char* pageDirectory)
{
if (results == NULL || pageDirectory == NULL) {
return;
}

// Make a copy of results to count matches
counters_t* resultsCopy = counters_new();
counters_iterate(results, resultsCopy, unionHelper);
// Count matching documents with non-zero scores
maxScore_t countData = {resultsCopy, 0, 0};
int numMatches = 0;
while (true) {
countData.maxScore = 0;
counters_iterate(resultsCopy, &countData, updateMaxScore);
if (countData.maxScore == 0) {
break;
}
numMatches++;
counters_set(resultsCopy, countData.maxDocID, 0);
}
counters_delete(resultsCopy);
// Print number of matches
printf("Matches %d documents (ranked):\n", numMatches);
// Find highest score
maxScore_t maxData = {results, 0, 0};
while (true) {
// go through the results and store the max rank
maxData.maxScore = 0;
counters_iterate(results, &maxData, updateMaxScore);
if (maxData.maxScore == 0) {
break;
}
// Print document with this score
printf("score: %4d doc: %3d: ", maxData.maxScore, maxData.maxDocID);
// Print URL from document
char* pathname = mem_malloc(strlen(pageDirectory) + 20);
sprintf(pathname, "%s/%d", pageDirectory, maxData.maxDocID);
FILE* fp = fopen(pathname, "r");
if (fp != NULL) {
char* url = file_readLine(fp);
if (url != NULL) {
printf("%s\n", url);
free(url);
}
fclose(fp);
}
mem_free(pathname);
// Zero out this document's score
counters_set(results, maxData.maxDocID, 0);
}
}


/**************** updateMaxScore ****************/
// method to update the max score
static void
updateMaxScore(void* arg, const int key, const int count)
{
maxScore_t* data = arg;
if (count > data->maxScore) {
data->maxScore = count;
data->maxDocID = key;
}
}

/**************** counter_count_helper ****************/
// Helper function to count nodes
void counter_count_helper(void* arg, const int key, const int count) {
int* total = (int*)arg;
(*total)++;
}

/**************** counters_count ****************/
// Function to count total number of counters
int counters_count(counters_t* ctrs) {
if (ctrs == NULL) {
    printf("Counters is null");
return 0;
}
int total = 0;
counters_iterate(ctrs, &total, counter_count_helper);
return total;
}