#include "simple_pipe.h"
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define BUf_SIZE 10

int simplePipe(int argc, char *argv[]) {
    int pfd[2];
    char buf[BUf_SIZE];
    ssize_t numRead;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s string\n", argv[0]);
        return (EXIT_FAILURE);
    }

    if (pipe(pfd) == -1) {
        fprintf(stderr, "failed to call pipe(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    switch (fork()) {
        case -1:
            fprintf(stderr, "failed to call folk(), %d\n", errno);
            exit(EXIT_FAILURE);
        case 0:
            _exit(EXIT_SUCCESS);
        default:
            exit(EXIT_SUCCESS);
    }
}