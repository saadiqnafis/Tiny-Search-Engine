#!/bin/bash

# Define paths and variables
pageDir="../crawler/data/letters-2"
validIndexFile="../crawler/data/letters-2.index"
readOnlyDir="../crawler/data"
readOnlyFile="../common/pagedit.h"
nonExistentDir="../data/nonexistent_dir"
indextestOutput="../crawler/data/letters-2-test.index"

# Ensure directories and files exist as expected
# mkdir -p $pageDir
# mkdir -p $readOnlyDir
# touch $readOnlyFile
# chmod 444 $readOnlyFile

echo "=== Testing indexer with invalid arguments ==="
# Test cases with invalid argument counts
echo "--- No arguments ---"
./indexer
echo ""
echo "--- One argument ---"
./indexer $pageDir
echo ""
echo "--- Three arguments ---"
./indexer $pageDir $validIndexFile extra_argument
echo ""
# Test cases with invalid pageDirectory and indexFile
echo "--- Invalid pageDirectory (non-existent path) ---"
./indexer $nonExistentDir $validIndexFile
echo ""
echo "--- Invalid pageDirectory (not a crawler directory) ---"
./indexer $readOnlyDir $validIndexFile
echo ""
echo "--- Invalid indexFile (non-existent path) ---"
./indexer $pageDir $nonExistentDir/nonexistent_index.index
echo ""
echo "--- Invalid indexFile (read-only directory) ---"
./indexer $pageDir $readOnlyDir/test_index.index
echo ""
echo "--- Invalid indexFile (existing, read-only file) ---"
./indexer $pageDir $readOnlyFile
echo ""
echo "=== Testing indexer on valid inputs ==="
# Run indexer on a variety of valid pageDirectories and validate with indextest
for depth in 0 1 2; do
    testIndexFile="../crawler/data/letters_${depth}.index"
    echo "--- Running indexer on $pageDir with depth $depth ---"
    ./indexer $pageDir $testIndexFile
    echo ""
    echo "--- Running indextest on the generated index file ---"
    ./indextest $testIndexFile $indextestOutput
    echo ""
    # Validate that the generated index file and the indextest output are equivalent
    ~/cs50-dev/shared/tse/indexcmp $testIndexFile $indextestOutput
    if [ $? -eq 0 ]; then
        echo "Index validation successful for depth $depth"
    else
        echo "Index validation failed for depth $depth"
    fi
    echo ""
done

echo ""
echo "=== Running valgrind to check for memory leaks in indexer and indextest ==="
# Run valgrind on indexer
valgrind --leak-check=full --error-exitcode=1 ./indexer $pageDir $validIndexFile
echo ""

# Run valgrind on indextest
valgrind --leak-check=full --error-exitcode=1 ./indextest $validIndexFile $indextestOutput
echo ""

# Cleanup files and directories
rm -f $readOnlyFile
rm -f $indextestOutput
for depth in 0 1 2; do
    rm -f "../crawler/data/letters_${depth}.index"
done

echo "Testing complete."