#!/bin/bash

test_dir=test

mkdir -p $test_dir/dir_a
mkdir -p $test_dir/dir_b

# Case 1:
echo "--- Case 1 ---"

# 1 common file, 1 unique file per directory
echo "common file" > $test_dir/dir_a/common.txt
cp $test_dir/dir_a/common.txt $test_dir/dir_b

echo "a file" > $test_dir/dir_a/afile.txt
echo "b file" > $test_dir/dir_b/bfile.txt

./build/compare $test_dir/dir_a $test_dir/dir_b

if grep -q "common.txt" output/common; then
    echo "common: PASSED"
else
    echo "common: FAILED"
fi

if grep -q "afile.txt" output/a_only; then
    echo "a only: PASSED"
else
    echo "a only: FAILED"
fi

if grep -q "bfile.txt" output/b_only; then
    echo "b only: PASSED"
else
    echo "b only: FAILED"
fi

rm -rf $test_dir

# Case 2: Empty Directories
echo "--- Case 2 ---"
mkdir -p $test_dir/dir_a
mkdir -p $test_dir/dir_b

./build/compare $test_dir/dir_a $test_dir/dir_b

# Common should be empty
if [[ ! -s output/common ]]; then
    echo "common: PASSED"
else
    echo "common: FAILED"
fi

# A should be empty
if [[ ! -s output/a_only ]]; then
    echo "a only: PASSED"
else
    echo "a only: FAILED"
fi

# B should be empty
if [[ ! -s output/b_only ]]; then
    echo "b only: PASSED"
else
    echo "b only: FAILED"
fi

rm -rf $test_dir

# Case 3: Non-standard file in a directory
echo "--- Case 3 ---"
mkdir -p $test_dir/dir_a
mkdir -p $test_dir/dir_b

echo "a file" > $test_dir/dir_a/afile.txt
ln -s $test_dir/dir_a/afile.txt $test_dir/dir_b/bfile.txt

./build/compare $test_dir/dir_a $test_dir/dir_b

# Common should be empty
if [[ ! -s output/common ]]; then
    echo "common: PASSED"
else
    echo "common: FAILED"
fi

# A should match the file
if grep -q "afile.txt" output/a_only; then
    echo "a only: PASSED"
else
    echo "a only: FAILED"
fi

# B should be empty
if [[ ! -s output/b_only ]]; then
    echo "b only: PASSED"
else
    echo "b only: FAILED"
fi

rm -rf $test_dir

# Case 4: Non-existent directory
echo "--- Case 4 ---"
mkdir -p $test_dir/dir_a

./build/compare $test_dir/dir_a $test_dir/dir_c 2>&1 >/dev/null

if [[ "$?" -eq 1 ]] 2>/dev/null; then
    echo "PASSED"
fi

rm -rf $test_dir