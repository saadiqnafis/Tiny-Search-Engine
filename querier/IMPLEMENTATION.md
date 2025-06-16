# CS50 Lab 6
## CS50 Fall 2024

### Implementation Specification for Querier

This document describes the implementation details of the ```querier``` module in the Tiny Search Engine (TSE), covering the main functions, key algorithms, and data handling.

#### Overview
The ```querier``` module loads an inverted index created by the ```indexer``` and uses it to process user queries. It supports ```and``` and ```or``` operators for combining search terms. The ```querier``` outputs matching documents sorted by relevance.

#### Key Functions
1. **main**
    * Parses command-line arguments for ```pageDirectory``` and ```indexFilename```.
    * Validates arguments and loads the inverted index from ```indexFilename``` into memory.
    * Starts an interactive query loop, prompting the user for search queries.
2. **processQueries**

    * Validates and tokenizes the user’s query.
    * Identifies and separates ```and``` and ```or``` sequences for processing.
    * Calls ```executeQuery``` to retrieve documents matching the query terms.

3. **validateQuery**

    * Checks for invalid syntax (e.g., consecutive **and**/**or**, operators at the start or end).
    * Ensures that only valid query structures are processed.

4. **executeQuery**

    * Iterates through query terms, handling ```and``` sequences by calling ```intersectCounters``` and ```or``` sequences by calling ```unionCounters```.
    * Returns a final set of document scores that match the query.

5. **intersectCounters**

    * For ```and``` sequences, it combines counters by retaining only documents common across all terms.
    * Accumulates scores based on each document’s minimum frequency across terms to ensure relevance.

6. **unionCounters**

    * For ```or``` sequences, it combines counters by including all documents across terms.
    * Adds document scores across terms to ensure relevance is reflected by combined occurrences.

7. **printDocumentRanking**

    * Sorts the matched documents by their relevance scores in descending order.
    * Outputs document IDs, URLs, and scores to the user.
    * If no documents match, displays “No documents match.”

#### Data Structures

* **Hashtable:** Used to store the inverted index, mapping each word to its associated ```counters```.
* **Counters:** Used to store document IDs and their frequency scores for each query term.

#### Query Processing Flow
1. **Tokenization and Validation:** ```processQueries``` tokenizes the input and validates the structure.
2. **Execution:** ```executeQuery``` evaluates the query using ```unionCounters``` and ```intersectCounters```.
3. **Display:** ```printDocumentRanking``` formats and outputs results based on sorted relevance.

#### Error Handling
* Errors in input arguments are flagged immediately in main.
* Query syntax errors are caught in validate_query to ensure only valid queries proceed.

#### Testing
Refer to ```DESIGN.md``` for a comprehensive testing plan covering command-line parsing, query syntax validation, document scoring, and output verification.