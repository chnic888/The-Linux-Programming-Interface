#define _POSIX_C_SOURCE 199309

#include "t_sigqueue.h"
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int tSigqueue(int argc, char *argv[]) {
    int sig, numSigs, j, sigData;
    union sigval sv;
    char *endPtr = NULL;

    if (argc < 4 || strcmp(argv[1], "--help ") == 0) {
        fprintf(stderr, "%s pid sig-num data [num-sigs]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "%s: PID is %ld, UID is %ld\n", argv[0], (long) getpid(), (long) getuid());

    sig = (int) strtol(argv[2], &endPtr, 10);
    endPtr = NULL;

    sigData = (int) strtol(argv[3], &endPtr, 10);
    endPtr = NULL;

    numSigs = (argc > 4) ? (int) strtol(argv[4], &endPtr, 10) : 1;
    endPtr = NULL;

    for (j = 0; j < numSigs; j++) {
        sv.sival_int = sigData + j;
        if (sigqueue(strtol(argv[1], &endPtr, 10), sig, sv) == -1) {
            fprintf(stderr, "failed to call (), %d, %d", j, errno);
            exit(EXIT_FAILURE);
        }
    }

    return (EXIT_SUCCESS);
}