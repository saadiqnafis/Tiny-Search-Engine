#!/bin/bash

# Here's the testing file for querier.c. We test many cases, including several edge cases
# Nafis Saadiq Bhuiyan

echo "=== Testing querier with invalid arguments ==="
# Test cases with invalid argument counts
echo "Test 1: No arguments "
./querier
echo ""
echo "Test 2: One argument"
./querier ~/cs50-dev/shared/tse/output/toscrape-3
echo ""
echo "Test 3: Three arguments"
./querier ~/cs50-dev/shared/tse/output/toscrape-3 ~/cs50-dev/shared/tse/output/toscrape-3.index extra_argument
echo ""

# to run: bash -v querier/testing.sh >& querier/testing.out

echo "Test 4: non crawler directory"
./querier ../libcs50 ~/cs50-dev/shared/tse/output/toscrape-3.index
echo ""
echo "Test 5: provided index isnt a file"
./querier ~/cs50-dev/shared/tse/output/toscrape-3 ~/cs50-dev/shared/tse/output/toscrape-3.index2
echo ""
echo "Test 6: read from stdin"
./querier ~/cs50-dev/shared/tse/output/toscrape-3 ~/cs50-dev/shared/tse/output/toscrape-3.index <<EOF
mom
EOF
echo ""
echo "Test 7: read in without and"
./querier ~/cs50-dev/shared/tse/output/toscrape-3 ~/cs50-dev/shared/tse/output/toscrape-3.index <<EOF
mom dad
EOF
echo ""
echo "Test 8: read in with and"
./querier ~/cs50-dev/shared/tse/output/toscrape-3 ~/cs50-dev/shared/tse/output/toscrape-3.index <<EOF
mom and dad
EOF
echo ""
echo "Test 9: read in with or"
./querier ~/cs50-dev/shared/tse/output/toscrape-3 ~/cs50-dev/shared/tse/output/toscrape-3.index <<EOF
mom or dad
EOF
echo ""
echo "Test 10: memory check"
valgrind --leak-check=full --show-leak-kinds=all ./querier ~/cs50-dev/shared/tse/output/toscrape-3 ~/cs50-dev/shared/tse/output/toscrape-3.index <<EOF
mom dad
EOF
echo ""
echo "Test 11: improper characters stdin"
./querier ~/cs50-dev/shared/tse/output/toscrape-3 ~/cs50-dev/shared/tse/output/toscrape-3.index <<EOF
mom & dad
EOF
echo ""
echo "Test 12: no stdin"
./querier ~/cs50-dev/shared/tse/output/toscrape-3 ~/cs50-dev/shared/tse/output/toscrape-3.index <<EOF

EOF
echo ""
echo "Test 13: start with and"
./querier ~/cs50-dev/shared/tse/output/toscrape-3 ~/cs50-dev/shared/tse/output/toscrape-3.index <<EOF
and mom and dad
EOF
echo ""
echo "Test 14: start with or"
./querier ~/cs50-dev/shared/tse/output/toscrape-3 ~/cs50-dev/shared/tse/output/toscrape-3.index <<EOF
or mom and dad
EOF
echo ""
echo "Test 15: end with or"
./querier ~/cs50-dev/shared/tse/output/toscrape-3 ~/cs50-dev/shared/tse/output/toscrape-3.index <<EOF
mom and dad or
EOF
echo ""
echo "Test 16: end with and"
./querier ~/cs50-dev/shared/tse/output/toscrape-3 ~/cs50-dev/shared/tse/output/toscrape-3.index <<EOF
mom and dad and
EOF
echo ""
echo "Test 17: put and and or next to eachother"
./querier ~/cs50-dev/shared/tse/output/toscrape-3 ~/cs50-dev/shared/tse/output/toscrape-3.index <<EOF
mom and or dad
EOF
echo ""
echo "Test 18: read in multiple lines"
./querier ~/cs50-dev/shared/tse/output/toscrape-3 ~/cs50-dev/shared/tse/output/toscrape-3.index <<EOF
mom or dad
mom
dad
mom and dad
mom and dd
mom dad
EOF
echo ""
echo "Test 19: line with one invalid entry"
./querier ~/cs50-dev/shared/tse/output/toscrape-3 ~/cs50-dev/shared/tse/output/toscrape-3.index <<EOF
mom dad dd
EOF
echo ""
echo "Test 20: word not in index"
./querier ~/cs50-dev/shared/tse/output/toscrape-3 ~/cs50-dev/shared/tse/output/toscrape-3.index <<EOF
jajjasdsa
EOF
echo ""