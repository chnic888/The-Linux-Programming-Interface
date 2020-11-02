#include "fcntl_get.h"
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int fcntlGet(int argc, char *argv[]) {
    if (argc != 2 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s pathname offset\n", argv[0]);
        return (EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "failed to open file %s, %d\n", argv[1], errno);
        return (EXIT_FAILURE);
    }

    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        fprintf(stderr, "failed to get file flag %s, %d\n", argv[1], errno);
        return (EXIT_FAILURE);
    }

    int accessMode = flags & O_ACCMODE;
    if (accessMode == O_RDONLY) {
        fprintf(stdout, "file is read only\n");
    } else {
        fprintf(stdout, "file is writable\n");
    }
    
    if (flags & O_SYNC) {
        fprintf(stdout, "writes are synchronized\n");
    } else {
        fprintf(stdout, "writes are asynchronized\n");
    }

    if (close(fd) == -1) {
        fprintf(stdout, "failed to close file %s, %d", argv[1], errno);
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}