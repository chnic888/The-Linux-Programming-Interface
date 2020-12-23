#include "view_symlink.h"
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE PATH_MAX

int viewSymlink(int argc, char *argv[]) {
    struct stat statuf;
    char buf[BUF_SIZE];
    ssize_t numBytes;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stdout, "%s pathname\n", argv[0]);
        return (EXIT_FAILURE);
    }

    if (lstat(argv[1], &statuf) == -1) {
        fprintf(stderr, "failed to call lstat(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (!S_ISLNK(statuf.st_mode)) {
        fprintf(stderr, "%s is not a symbolic link\n", argv[1]);
        return (EXIT_FAILURE);
    }

    numBytes = readlink(argv[1], buf, BUF_SIZE - 1);
    if (numBytes == -1) {
        fprintf(stderr, "failed to call readlink(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    buf[numBytes] = '\0';
    fprintf(stdout, "readlink: %s --> %s\n", argv[1], buf);

    if (realpath(argv[1], buf) == NULL) {
        fprintf(stderr, "failed to call realpath(), %d\n", errno);
        return (EXIT_FAILURE);
    }
    fprintf(stdout, "realpath: %s --> %s\n", argv[1], buf);

    return (EXIT_SUCCESS);
}