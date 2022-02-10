#define  _DEFAULT_SOURCE

#include "memlock.h"
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

static void displayMincore(char *addr, size_t length) {
    unsigned char *vec;
    long pageSize, numPages, j;

    pageSize = sysconf(_SC_PAGESIZE);

    numPages = ((long) length + pageSize - 1) / pageSize;
    vec = malloc(numPages);

    if (vec == NULL) {
        fprintf(stderr, "failed to call malloc(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if (mincore(addr, length, vec) == -1) {
        fprintf(stderr, "failed to call mincore(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    for (j = 0; j < numPages; j++) {
        if (j % 64 == 0) {
            fprintf(stdout, "%s%10p: ", (j == 0) ? "" : "\n", addr + (j * pageSize));
        }
        fprintf(stdout, "%c", (vec[j] & 1) ? '*' : '.');
    }

    fprintf(stdout, "\n");
    free(vec);
}


int memlock(int argc, char *argv[]) {
    char *addr;
    size_t len, lockLen;
    long pageSize, stepSize, j;
    char *endPtr = NULL;

    if (argc != 4 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s num-steps lock-page-step lock-page-len\n", argv[0]);
        return (EXIT_FAILURE);
    }

    pageSize = sysconf(_SC_PAGESIZE);
    if (pageSize == -1) {
        fprintf(stderr, "failed to call sysconf(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    len = strtol(argv[1], &endPtr, 10) * pageSize;
    endPtr = NULL;

    stepSize = strtol(argv[2], &endPtr, 10) * pageSize;
    endPtr = NULL;

    lockLen = strtol(argv[3], &endPtr, 10) * pageSize;
    endPtr = NULL;

    addr = mmap(NULL, len, PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED) {
        fprintf(stderr, "failed to call mmap(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Allocated %ld (%#lx) bytes starting at %p\n", (long) len, (unsigned long) len, addr);
    fprintf(stdout, "Before mlock:\n");
    displayMincore(addr, len);

    for (j = 0; j + lockLen <= len; j += stepSize) {
        if (mlock(addr + j, lockLen) == -1) {
            fprintf(stderr, "failed to call mlock(), %d\n", errno);
            return (EXIT_FAILURE);
        }
    }

    fprintf(stdout, "After mlock:\n");
    displayMincore(addr, len);

    return (EXIT_SUCCESS);
}