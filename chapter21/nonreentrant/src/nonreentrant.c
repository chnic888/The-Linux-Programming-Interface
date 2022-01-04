#define _GNU_SOURCE

#include "nonreentrant.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
//#include <crypt.h>
#include <unistd.h>

static char *str2;
static int handled = 0;

static void handler(int sig) {
    crypt(str2, "xx");
    handled++;
}

int nonreentrant(int argc, char *argv[]) {
    char *cr1;
    int callNum, mismatch;
    struct sigaction sa;

    if (argc != 3) {
        fprintf(stderr, "%s str1 str2\n", argv[0]);
        return (EXIT_FAILURE);
    }

    str2 = argv[2];
    cr1 = strdup(crypt(argv[1], "xx"));

    if (cr1 == NULL) {
        fprintf(stderr, "failed to call strdup(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    for (callNum = 1, mismatch = 0;; callNum++) {
        if (strcmp(crypt(argv[1], "xx"), cr1) != 0) {
            mismatch++;
            fprintf(stdout, "Mismatch on call %d (mismatch=%d handled=%d)\n", callNum, mismatch, handled);
        }
    }
}