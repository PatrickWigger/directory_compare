#!/bin/bash

########################
# Large File Test
########################

# Test directories
DIR_A="test/dir_a"
DIR_B="test/dir_b"
OUTPUT="output"
mkdir -p $DIR_A
mkdir -p $DIR_B

make clean && make

# Create 90,000 common files
for i in $(seq 1 90000); do
    echo "common file" > "$DIR_A/file_$i.txt"
    cp "$DIR_A/file_$i.txt" "$DIR_B/file_$i.txt"
done

# Create 5,000 a only files
for i in $(seq 90001 95000); do
    echo "a specific file" > "$DIR_A/file_$i.txt"
done

# Create 5,000 b only files
for i in $(seq 95001 100000); do
    echo "b specific file" > "$DIR_B/file_$i.txt"
done

./build/compare $DIR_A $DIR_B

# Check each output for appropriate number of files
if [[ $(wc -l < "$OUTPUT/common") -eq 90000 ]]; then
    echo "common: PASSED"
else
    echo "common: FAILED"
fi

if [[ $(wc -l < "$OUTPUT/a_only") -eq 5000 ]]; then
    echo "a only: PASSED"
else
    echo "a only: FAILED"
fi

if [[ $(wc -l < "$OUTPUT/b_only") -eq 5000 ]]; then
    echo "b only: PASSED"
else
    echo "b only: FAILED"
fi