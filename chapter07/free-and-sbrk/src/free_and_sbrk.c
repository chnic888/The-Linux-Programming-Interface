#include "free_and_sbrk.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define MAX_ALLOCS 1000000

static int getInt(const char *, int);

int freeSbrk(int argc, char *argv[]) {
    char *ptr[MAX_ALLOCS];
    int freeStep, freeMin, freeMax, blockSize, numAllocs, j;

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

    freeStep = (argc > 3) ? getInt(argv[3], GN_GT_O) : 1;
    freeMin = (argc > 4) ? getInt(argv[4], GN_GT_O) : 1;
    freeMax = (argc > 5) ? getInt(argv[5], GN_GT_O) : numAllocs;

    if (freeMax > numAllocs) {
        fprintf(stderr, "free-max > num-allocs\n");
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Initial program break:         %10p\n", sbrk(0));

    fprintf(stdout, "Allocating %d*%d bytes\n", numAllocs, blockSize);
    for (j = 0; j < numAllocs; j++) {
        ptr[j] = malloc(blockSize);
        if (ptr[j] == NULL) {
            fprintf(stderr, "failed to malloc\n");
            return (EXIT_FAILURE);
        }
    }

    fprintf(stdout, "Program break is now:         %10p\n", sbrk(0));

    fprintf(stdout, "freeing blocks from %d to %d in steps of %d\n", freeMin, freeMax, freeStep);
    for (j = freeMin - 1; j < freeMax; j+= freeStep) {
        free(ptr[j]);
    }

    fprintf(stdout, "After free(), program break is:   %10p\n", sbrk(0));

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