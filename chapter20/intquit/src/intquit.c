#include "intquit.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

static void sigHandler(int);

int intquit(int argc, char *argv[]) {
    if (signal(SIGINT, sigHandler) == SIG_ERR) {
        fprintf(stderr, "signal fialed, %d", errno);
        exit(EXIT_FAILURE);
    }

    if (signal(SIGQUIT, sigHandler) == SIG_ERR) {
        fprintf(stderr, "signal fialed, %d", errno);
        exit(EXIT_FAILURE);
    }

    for (;;) {
        fprintf(stdout, "Aha, ready to pause\n");
        pause();
    }

}

static void sigHandler(int sig) {
    static int count = 0;

    if (sig == SIGINT) {
        count++;
        fprintf(stdout, "Caught SINGINT (%d)\n", count);
        return;
    }

    fprintf(stdout, "Caught SIGQUIT (%d) - that's all folks!\n", count);
    exit(EXIT_SUCCESS);
}