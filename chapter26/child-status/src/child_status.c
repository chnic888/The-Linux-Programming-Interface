#include "child_status.h"
#include "print_wait_status.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int childStatus(int argc, char *argv[]) {
    int status;
    pid_t childPid;
    char *endPtr = NULL;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        fprintf(stdout, "%s [exit-status]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    switch (fork()) {
        case -1:
            fprintf(stderr, "failed to call fork(), %d\n", errno);
            exit(EXIT_SUCCESS);
        case 0:
            fprintf(stdout, "Child started with PID = %ld\n", (long) getpid());
            if (argc > 1) {
                exit((int) strtol(argv[1], &endPtr, 10));
            } else {
                for (;;) {
                    pause();
                }
            }
        default:
            for (;;) {
                childPid = waitpid(-1, &status, WUNTRACED
                                                #ifdef WCONTINUED
                                                | WCONTINUED
#endif
                );

                if (childPid == -1) {
                    fprintf(stderr, "failed to call waitpid(), %d\n", errno);
                    exit(EXIT_FAILURE);
                }

                fprintf(stdout, "waitpid() returned: PID=%ld; status=0x%04x (%d,%d)\n", (long) childPid,
                        (unsigned int) status, status >> 8, status & 0xff);
                printWaitStatus(NULL, status);

                if (WIFEXITED(status) || WIFSIGNALED(status)) {
                    exit(EXIT_SUCCESS);
                }
            }

    }
}