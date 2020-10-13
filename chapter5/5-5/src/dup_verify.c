#include "dup_verify.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

void printFdOffsetInfo(int, int);

void printFdFlagInfo(int, int);

int dupVerify(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stdout, "%s filename num-bytes\n", argv[0]);
        return (EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDWR | O_APPEND);
    if (fd == -1) {
        fprintf(stderr, "failed to open file %s, %d\n", argv[1], errno);
        return (EXIT_FAILURE);
    }

    int dupFd = dup(fd);
    if (dupFd == -1) {
        fprintf(stderr, "failed to dup file descriptor\n");
        return (EXIT_FAILURE);
    }

    printFdOffsetInfo(fd, dupFd);

    char *end = argv[2] + strlen(argv[2]);
    long size = strtol(argv[2], &end, 0);

    char buf[size];
    ssize_t readNum = read(fd, buf, size);
    fprintf(stdout, "read %ld bytes\n", readNum);

    printFdOffsetInfo(fd, dupFd);
    printFdFlagInfo(fd, dupFd);

    if (close(fd) == -1) {
        fprintf(stderr, "failed to close fd, %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (close(dupFd) == -1) {
        fprintf(stderr, "failed to close dupFd, %d\n", errno);
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

void printFdOffsetInfo(int fd, int dupFd) {
    off_t fdOffset = lseek(fd, 0, SEEK_CUR);
    off_t dupFdOffset = lseek(dupFd, 0, SEEK_CUR);

    fprintf(stdout, "fd offset %lld, dupFd offset %lld\n", fdOffset, dupFdOffset);
}

void printFdFlagInfo(int fd, int dupFd) {
    int fdFlags = fcntl(fd, F_GETFL);
    int dupFdFlags = fcntl(dupFd, F_GETFL);

    int fdAccessMode = fdFlags & O_ACCMODE;
    int dubFdAccessMode = dupFdFlags & O_ACCMODE;

    if (fdAccessMode == O_RDONLY) {
        fprintf(stdout, "fd is read only mode\n");
    } else {
        fprintf(stdout, "fd is not read only mode\n");
    }

    if (dubFdAccessMode == O_RDONLY) {
        fprintf(stdout, "dupFd is read only mode\n");
    } else {
        fprintf(stdout, "dupFd is not read only mode\n");
    }
}
