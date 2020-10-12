#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "atomic_append.h"

int atomicAppend(int argc, char *argv[]) {
    if (argc != 3 && argc != 4) {
        fprintf(stderr, "%s filename num-bytes [x]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    int fd, flags = O_WRONLY;

    if (argc == 3) {
        flags |= O_APPEND;
    }

    fd = open(argv[1], flags);
    if (fd == -1) {
        fprintf(stderr, "failed to open file %s, %d\n", argv[1], errno);
        return (EXIT_FAILURE);
    }

    char *end = argv[2] + strlen(argv[2]);
    size_t length = strtoll(argv[2], &end, 0);

    char *str = malloc(length * sizeof(char));
    for (int i = 0; i < length; i++) {
        strcat(str, "A");
    }

    if (argc == 4) {
        if (lseek(fd, 0, SEEK_END) == -1) {
            fprintf(stderr, "failed to seek the end of file, %d\n", errno);
            return (EXIT_FAILURE);
        }
    }

    if (write(fd, str, length) == -1) {
        fprintf(stderr, "failed to write %ld bytes, %d\n", length, errno);
        return (EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        fprintf(stderr, "failed to close file %s, %d\n", argv[1], errno);
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

