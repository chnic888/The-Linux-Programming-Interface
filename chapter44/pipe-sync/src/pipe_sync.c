#include "pipe_sync.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 10

int pipeSync(int argc, char *argv[]) {
    int pfd[2];
    int j;
    char buf[BUF_SIZE];

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s sleep-time...\n", argv[0]);
        return (EXIT_FAILURE);
    }

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    fprintf(stdout, "[parent - 1] %d\n", getpid());
    fprintf(stdout, "[parent]%ld     Parent started\n", time(NULL));

    if (pipe(pfd) == -1) {
        fprintf(stderr, "failed to call pipe(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    for (j = 1; j < argc; j++) {
        switch (fork()) {
            case -1:
                fprintf(stderr, "failed to call folk(), %d\n", errno);
                exit(EXIT_FAILURE);

            case 0:
                if (close(pfd[0] == -1)) {
                    fprintf(stderr, "[child -%d]failed to close the read fd, %d\n", j, errno);
                    exit(EXIT_FAILURE);
                }

                char *endPtr = NULL;
                sleep(strtol(argv[j], &endPtr, 10));
                write(pfd[1], "Hello", 5);

                fprintf(stdout, "[child -%d]%ld  Child %d (PID=%d) closing pipe\n", j, time(NULL), j, getpid());
                if (close(pfd[1]) == -1) {
                    fprintf(stderr, "[child -%d]failed to close the write fd, %d\n", j, errno);
                    exit(EXIT_FAILURE);
                }

                exit(EXIT_SUCCESS);

            default:
                fprintf(stdout, "[parent - 1-%d] %d\n", j, getpid());
                break;
        }
    }

    if (close(pfd[1] == -1)) {
        fprintf(stderr, "[parent]failed to close the write fd, %d\n", errno);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "[parent - 2] %d\n", getpid());
    ssize_t num = read(pfd[0], buf, BUF_SIZE);

    fprintf(stdout, "[parent]read %zd\n", num);
    fprintf(stdout, "[parent]%ld     Parent ready to go\n", time(NULL));
    return (EXIT_SUCCESS);
}