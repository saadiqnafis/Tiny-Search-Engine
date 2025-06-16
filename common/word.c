/**
 * word - CS50 word module
 * 
 *We create a re-usable module word.c to normalize the words
 * 
 * Nafis Saadiq Bhuiyan, October 2024
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "word.h"

// Normalizes the word by converting all letters to lowercase
char* to_lowercase(const char* word) 
{
    // Allocate memory for the new lowercase string
    char* lower_word = malloc(strlen(word) + 1);
    if (lower_word == NULL) {
        return NULL;
    }

    // Convert each character to lowercase
    for (size_t i = 0; word[i]; i++) {
        lower_word[i] = tolower((unsigned char)word[i]);
    }
    lower_word[strlen(word)] = '\0';

    return lower_word;
}
