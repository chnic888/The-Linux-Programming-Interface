#include "fork_whos_on_first.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int forkWhosOnFirst(int argc, char *argv[]) {
    int numChildren, j;
    pid_t childPid;
    char *endPtr = NULL;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s [num-children]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    numChildren = (argc > 1) ? (int) strtol(argv[1], &endPtr, 10) : 1;
    setbuf(stdout, NULL);

    for (j = 0; j < numChildren; j++) {
        switch (childPid = fork()) {
            case -1:
                fprintf(stderr, "failed to call fork(), %d\n", errno);
                exit(EXIT_FAILURE);
            case 0:
                fprintf(stdout, "%d child\n", j);
                _exit(EXIT_SUCCESS);
            default:
                fprintf(stdout, "%d parent\n", j);
                wait(NULL);
        }
    }

    return (EXIT_SUCCESS);
}