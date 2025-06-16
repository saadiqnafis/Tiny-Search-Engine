#!/bin/bash
# testing.sh
# Script to test the crawler program with various command-line arguments
# Nafis Saadiq Bhuiyan, October 2024

# Define directories for test output
mkdir -p data
mkdir -p data/letters data/toscrape
mkdir -p data/letters-0
mkdir -p data/letters-1
mkdir -p data/letters-2

mkdir -p data/toscrape-0
mkdir -p data/toscrape-1


# Run with erroneous arguments
echo "Running tests with erroneous arguments..."
echo "Test Missing depth argument"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ data/letters      # Missing depth argument
echo "Test Invalid Directory"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ not_a_directory 1  # Invalid directory
echo "Test Invalid URL"
./crawler not_a_url data/letters 1                                        # Invalid URL
echo "Test Invalid Depth"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ data/letters -1    # Invalid depth
echo "Test out of range depth"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ data/letters 11    # Depth out of range (max is 10)
echo "Error testing complete."

# # Run with Valgrind over a moderate-sized test case
# echo "Running with Valgrind on toscrape at depth 1..."
# valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/letters-1 1

# Run crawler over CS50 websites
echo "Testing CS50 websites with various depths..."

# Letters website
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/letters-0 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/letters-1 1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/letters-2 2
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/letters-10 10

# # Toscrape website
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html data/toscrape-0 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html data/toscrape-1 1
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html data/toscrape-2 2
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html data/toscrape-3 3

# # Wikipedia website
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html data/wikipedia-0 0
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html data/wikipedia-1 1
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html data/wikipedia-2 2

echo "Testing complete."

# Inspect the output directories to verify the crawler saved files correctly
echo "Verifying output in data directories..."
ls -l data/letters-*
echo ""
ls -l data/toscrape-*
# ls -l data/wikipedia-*

echo "Verification complete."
