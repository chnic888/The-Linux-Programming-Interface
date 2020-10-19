#include "free_and_sbrk.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAX_ALLOCS 1000000

static int getInt(const char *, int);

int freeSbrk(int argc, char *argv[]) {
    char *ptr[MAX_ALLOCS];
    int freeStrep, freeMin, freeMax, blockSize, numAllocs, j;

    fprintf(stdout, "\n");

    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s num-allocs block-size [step [min [max]]]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    numAllocs = getInt(argv[1], GN_GT_O);
    if (numAllocs > MAX_ALLOCS) {
        fprintf(stderr, "num-allocs > %d\n", MAX_ALLOCS);
        return (EXIT_FAILURE);
    }

    blockSize = getInt(argv[2], GN_GT_O | GN_ANY_BASE);

    fprintf(stdout, "%d\n", numAllocs);
    fprintf(stdout, "%d\n", blockSize);

    return (EXIT_SUCCESS);
}

static int getInt(const char *arg, int flags) {
    char *endptr = NULL;
    int base = (flags & GN_ANY_BASE) ? 0 : (flags & GN_BASE_8) ? 8 : (flags & GN_BASE_16) ? 16 : 10;

    errno = 0;
    long res = strtol(arg, &endptr, base);

    if (errno != 0) {
        fprintf(stdout, "strtol() failed, %d", errno);
        return (EXIT_FAILURE);
    }

    return (int) res;
}