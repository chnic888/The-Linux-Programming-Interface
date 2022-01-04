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
    int j, dummy;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s sleep-time...\n", argv[0]);
        return (EXIT_FAILURE);
    }

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    fprintf(stdout, "[parent - %d]%ld     Parent started\n", getpid(), time(NULL));

    if (pipe(pfd) == -1) {
        fprintf(stderr, "failed to call pipe(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    for (j = 1; j < argc; j++) {
        switch (fork()) {
            case -1:
                fprintf(stderr, "%d failed to call folk(), %d\n", j, errno);
                exit(EXIT_FAILURE);

            case 0:
                if (close(pfd[0] == -1)) {
                    fprintf(stderr, "[child -%d]failed to close the read fd, %d\n", j, errno);
                    exit(EXIT_FAILURE);
                }

                char *endPtr = NULL;
                long second = strtol(argv[j], &endPtr, 10);
                fprintf(stderr, "[child -%d]%ld ready to sleep %ld second\n", getpid(), time(NULL), second);
                sleep(((int) second) * 10);

                if (close(pfd[1]) == -1) {
                    fprintf(stderr, "[child -%d]failed to close the write fd, %d\n", j, errno);
                    exit(EXIT_FAILURE);
                }
                fprintf(stdout, "[child- %d]%ld  Child %d closing pipe\n", getpid(), time(NULL), j);

                _exit(EXIT_SUCCESS);

            default:
                break;
        }
    }

    if (close(pfd[1] == -1)) {
        fprintf(stderr, "[parent]failed to close the write fd, %d\n", errno);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "[parent - %d] do some work A\n", getpid());
    sleep(3);
    fprintf(stdout, "[parent - %d] do some work B\n", getpid());
    sleep(3);
    fprintf(stdout, "[parent - %d] do some work C\n", getpid());
    sleep(3);

    if (read(pfd[0], &dummy, 1) != 0) {
        fprintf(stderr, "[parent] did not get EOF, %d\n", errno);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "[parent]%ld     Parent ready to go\n", time(NULL));
    return (EXIT_SUCCESS);
}