#include "t_unlink.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define CMD_SIZE 200
#define BUF_SIZE 1024

int tUnlink(int argc, char *argv[]) {
    int fd, j, numBlocks;
    char shellCmd[CMD_SIZE];
    char buf[BUF_SIZE];
    char *endPtr;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s temp-file [num-1kb-block] \n", argv[0]);
        return (EXIT_FAILURE);
    }

    numBlocks = argc > 2 ? (int) strtol(argv[2], &endPtr, 10) : 100000;
    fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        fprintf(stderr, "failed to open file %s, %d\n", argv[1], errno);
        return (EXIT_FAILURE);
    }

    if (unlink(argv[1]) == -1) {
        fprintf(stderr, "failed to call unlink, %d\n", errno);
        return (EXIT_FAILURE);
    }

    for (j = 0; j < numBlocks; j++) {
        if (write(fd, buf, BUF_SIZE) != BUF_SIZE) {
            fprintf(stderr, "partial/failed write %d\n", errno);
            return (EXIT_FAILURE);
        }
    }

    snprintf(shellCmd, CMD_SIZE, "df -k `dirname %s`", argv[1]);
    system(shellCmd);

    if (close(fd) == -1) {
        fprintf(stderr, "failed to call close(), %d\n", errno);
        return (EXIT_FAILURE);
    }
    fprintf(stdout, "********** Closed file descriptor\n");

    system(shellCmd);
    return (EXIT_SUCCESS);
}