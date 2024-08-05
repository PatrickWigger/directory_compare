


## Directory Comparison
### Requirements:
- gcc (clang 15.0.0). Compiled on MacOS.
- openssl library. For MacOS: `brew install openssl`

### Project Structure
```bash
- include
    - compare.h
- output
    - (holds generated output files)
- src
    - main.c
    - compare.c
- Makefile
- README.md
- test_large.sh (test 100,000 files)
- test.sh (test sample cases)
```

### Build
```bash
make
```
### Run
With Makefile:
```bash
make run ARGS="dirA dirB"
```
With command line:
```bash
./build/compare dirA dirB
```
# Test
With Makefile
```bash
make test
```
With command line
```bash
./test.sh
./test_large.sh (Takes some time)
```
### Clean
```bash
make clean
```