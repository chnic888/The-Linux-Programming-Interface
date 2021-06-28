#include "pipe_ls_wc.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

int pipeLsWc(int argc, char *argv[]) {
    int pfd[2];

    if (pipe(pfd) == -1) {
        fprintf(stderr, "failed to call pipe(), %d", errno);
        exit(EXIT_FAILURE);
    }

    switch (fork()) {
        case -1:
            fprintf(stderr, "failed to call folk() 1, %d\n", errno);
            exit(EXIT_FAILURE);

        case 0:
            if (close(pfd[0]) == -1) {
                fprintf(stderr, "[child]failed to close the read fd, %d\n", errno);
                exit(EXIT_FAILURE);
            }

            if (pfd[1] != STDOUT_FILENO) {
                if (dup2(pfd[1], STDOUT_FILENO) == -1) {
                    fprintf(stderr, "[child]failed to call dup2(), %d\n", errno);
                    exit(EXIT_FAILURE);
                }

                if (close(pfd[1]) == -1) {
                    fprintf(stderr, "[child]failed to call close(), %d\n", errno);
                    exit(EXIT_FAILURE);
                }

                execlp("ls", "ls", (char *) NULL);
                exit(EXIT_FAILURE);
            }

        default:
            break;
    }

    switch (fork()) {
        case -1:
            fprintf(stderr, "failed to call folk() 2, %d\n", errno);
            exit(EXIT_FAILURE);

        case 0:
            if (close(pfd[1]) == -1) {
                fprintf(stderr, "[child]failed to close the write fd, %d\n", errno);
                exit(EXIT_FAILURE);
            }

            if (pfd[0] != STDIN_FILENO) {
                if (dup2(pfd[0], STDIN_FILENO) == -1) {
                    fprintf(stderr, "[child]failed to call dup2(), %d\n", errno);
                    exit(EXIT_FAILURE);
                }

                if (close(pfd[0]) == -1) {
                    fprintf(stderr, "[child]failed to call close(), %d\n", errno);
                    exit(EXIT_FAILURE);
                }
            }

            execlp("wc", "wc", "-l", (char *) NULL);
            exit(EXIT_FAILURE);
        default:
            break;
    }

    if (close(pfd[0]) == -1) {
        fprintf(stderr, "failed to close read fd, %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if (close(pfd[1]) == -1) {
        fprintf(stderr, "failed to close write fd, %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if (wait(NULL) == -1) {
        fprintf(stderr, "failed to call wait(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if (wait(NULL) == -1) {
        fprintf(stderr, "failed to call wait(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}