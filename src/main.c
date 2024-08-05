#include "compare.h"
#include <stdio.h>


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <dirA> <dirB>\n", argv[0]);
        return 1;
    }

    const char *dirA = argv[1];
    const char *dirB = argv[2];
    const char *output = "output";
    compareDirectories(dirA, dirB, output);

    return 0;
}