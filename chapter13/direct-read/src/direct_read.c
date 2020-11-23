#define _GNU_SOURCE

#include "direct_read.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int directRead(int argc, char *argv[]) {
    int fd;
    ssize_t numRead;
    size_t length, alignment;
    off_t offset;
    void *buf;
    char *endPtr;

    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s file length [offset [alignment]]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    length = strtol(argv[2], &endPtr, 10);

    endPtr = NULL;
    offset = argc > 3 ? strtol(argv[3], &endPtr, 10) : 0;

    endPtr = NULL;
    alignment = argc > 4 ? strtol(argv[4], &endPtr, 10) : 4096;

    fd = open(argv[1], O_RDONLY | O_DIRECT);
    if (fd == -1) {
        fprintf(stderr, "failed to open file %s, %d\n", argv[1], errno);
        return (EXIT_FAILURE);
    }


    return (EXIT_SUCCESS);
}