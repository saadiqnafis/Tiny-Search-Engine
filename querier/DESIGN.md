# CS50 Lab 6
## CS50 Fall 2024

### Design Specification for Querier
In this document, we outline the design of the ```querier``` module for the Tiny Search Engine (TSE). This document includes an implementation-independent view of the module, focusing on user interface, inputs and outputs, functional decomposition, algorithm flow, major data structures, and a testing plan.

### User Interface
The querier has a command-line interface with the following format:

```bash
$ ./querier pageDirectory indexFilename
```
* **pageDirectory:** The directory where the ```crawler``` has stored the web pages, each page saved with a unique document ID.
* **indexFilename:** The file where the ```indexer``` stored the word frequencies per document.

After initializing, the querier prompts the user for queries in an interactive shell:
```Query?```

Users can enter keyword queries, using operators like ```and``` and ```or``` to define search criteria. The querier processes these queries and displays matching document results.

### Inputs and Outputs

* #### Inputs:

    * #### Command-line parameters:
        * ```pageDirectory``` - directory with the files from the crawler.
        * ```indexFilename``` - file containing the inverted index of word-document frequencies.
    * **User Query:** keyword query with optional ```and```/```or``` operators, entered interactively.
* #### Outputs:
    * Displays the search results for each query, listing document IDs and relevance scores for matched pages. If no documents match, outputs "No documents match."

### Functional Decomposition into Modules
1. **Main Function:** Parses command-line arguments, validates input, and initializes modules.
2. **processQueries:** Manages the parsing, validation, and processing of each user query.
3. **validateQuery:** Checks query syntax for errors, such as adjacent operators or trailing operators.
4. **executeQuery:** Evaluates the query and returns a set of matching documents.
5. **unionCounters:** Merges document scores for OR operations.
6. **intersectCounters:** Finds common documents and combines scores for AND operations.
7. **printDocumentRanking:** Sorts and prints the results of a query based on relevance.

### Major Data Structures
* **Inverted Index:** Stores the occurrences of each word in each document, mapping words to counters of document IDs and their frequencies.
* **Counters:** Holds document IDs and relevance scores for each query.
* **Data Structures Used in Query Processing:**
    * ```counters_union``` and ```counters_intersect``` use counters to combine document results by adding or intersecting scores based on query logic.
### Testing Plan
1. **Input Validation:**

    * Test with incorrect arguments (e.g., missing or invalid ```pageDirectory``` and ```indexFilename```).
    * Test queries with invalid syntax, such as ```and or```, ```or```, ```or and``` at the beginning or end of a query.
2. **Query Evaluation:**
    * **Basic Queries:** Single-word queries to ensure that documents containing the word are returned with correct scores.
    * **AND Queries:** Multiple words separated by and to verify that only documents containing all terms are returned.
    * **OR Queries:** Multiple words separated by or to ensure that documents containing any of the words are returned.
    * **Mixed Queries:** Test combinations of and and or operators in the same query to verify correct precedence.
3. **Output Verification:**
    * Confirm that output displays document IDs and relevance scores sorted by relevance.
    * Test various queries to ensure that results match the expected relevance based on the index file.
    * Verify that "No documents match" is output if no documents satisfy the query.
4. **Edge Cases:**
    * Query terms that do not appear in any document.
    * Queries that contain only ```and``` or ```or```.
    * Large numbers of terms to test performance and memory use.
    * Test queries with high document scores to verify correct sorting and display.
5. **Functional Testing with Sample Data:**

    * Use a small test corpus to manually check that the results match expected document relevance.
    * Increase complexity with larger datasets to ensure that the querier correctly ranks documents by relevance based on word frequency and logical operators.
