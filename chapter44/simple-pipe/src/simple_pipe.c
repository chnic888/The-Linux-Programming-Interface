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
            if (close(pfd[1]) == -1) {
                fprintf(stderr, "[child]failed to clod the write fd, %d\n", errno);
                exit(EXIT_FAILURE);
            }

            for (;;) {
                numRead = read(pfd[0], buf, BUf_SIZE);
                if (numRead == -1) {
                    fprintf(stderr, "[child]failed to clod the write fd, %d\n", errno);
                    exit(EXIT_FAILURE);
                }

                if (numRead == 0) {
                    break;
                }

                if (write(STDOUT_FILENO, buf, numRead) != numRead) {
                    fprintf(stderr, "[child]failed to write string to stdout, %d\n", errno);
                    exit(EXIT_FAILURE);
                }
            }

            write(STDOUT_FILENO, "\n", 1);
            if (close(pfd[0]) == -1) {
                fprintf(stderr, "[child]failed to clod the read fd, %d\n", errno);
                exit(EXIT_FAILURE);
            }

            _exit(EXIT_SUCCESS);

        default:
            if (close(pfd[0]) == -1) {
                fprintf(stderr, "[parent]failed to clod the read fd, %d\n", errno);
                exit(EXIT_FAILURE);
            }

            if (write(pfd[1], argv[1], strlen(argv[1])) != strlen(argv[1])) {
                fprintf(stderr, "[parent]failed to write the pipe, %d\n", errno);
                exit(EXIT_FAILURE);
            }

            if (close(pfd[1]) == -1) {
                fprintf(stderr, "[parent]failed to clod the write fd, %d\n", errno);
                exit(EXIT_FAILURE);
            }

            wait(NULL);
            exit(EXIT_SUCCESS);
    }
}