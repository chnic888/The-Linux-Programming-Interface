#include "multi_wait.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

int multiWait(int argc, char *argv[]) {
    int numDead;
    pid_t childPid;
    int j;
    char *endPtr = NULL;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s sleep-time...\n", argv[0]);
        return (EXIT_FAILURE);
    }

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    for (j = 1; j < argc; j++) {
        switch (fork()) {
            case -1:
                fprintf(stderr, "failed to call fork(), %d\n", errno);
                exit(EXIT_FAILURE);
            case 0:
                fprintf(stdout, "[%ld] child %d started with PID %ld, sleeping %s seconds\n", time(NULL), j,
                        (long) getpid(), argv[j]);

                int sec = (int) strtol(argv[j], &endPtr, 10);
                fprintf(stdout, "ready to sleep %d seconds\n", sec);
                sleep(sec);
                _exit(EXIT_SUCCESS);

            default:
                break;
        }
    }

    numDead = 0;
    for (;;) {
        childPid = wait(NULL);
        if (childPid == -1) {
            if (errno == ECHILD) {
                fprintf(stdout, "no more child process - bye!\n");
                exit(EXIT_SUCCESS);
            } else {
                fprintf(stderr, "failed to call wait(), %d\n", errno);
                exit(EXIT_FAILURE);
            }
        }

        numDead++;
        fprintf(stdout, "[%ld] wait() returned child PID %ld (numDead=%d)\n", time(NULL), (long) childPid, numDead);
    }

}