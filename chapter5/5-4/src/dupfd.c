#include "dupfd.h"
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int dupfd1(int);

void printOffset(int, int);

int dupfd(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s filename\n", argv[0]);
        return (EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);
    int dupFd = dupfd1(fd);

    if (dupFd == -1) {
        return (EXIT_FAILURE);
    }

    close(fd);
    close(dupFd);

    return (EXIT_SUCCESS);
}

int dupfd1(int fd) {
    int dupFd = fcntl(fd, F_DUPFD, fd);
    if (dupFd == -1) {
        fprintf(stderr, "failed to dup fd, %d\n", errno);
        return dupFd;
    }

    printOffset(fd, dupFd);

    char buf[10];
    read(fd, buf, 10);

    printOffset(fd, dupFd);
    return dupFd;
}

void printOffset(int fd, int dupFd) {
    off_t fdOffset = lseek(fd, 0, SEEK_CUR);
    off_t dupFdOffset = lseek(dupFd, 0, SEEK_CUR);
    fprintf(stdout, "fd offset %lld, dupFd offset %lld\n", fdOffset, dupFdOffset);
}