#define _DEFAULT_SOURCE

#include "exit_handlers.h"
#include <stdlib.h>
#include <stdio.h>

static void atexitFunc1(void) {
    fprintf(stdout, "atexit function 1 called\n");
}

static void atexitFunc2(void) {
    fprintf(stdout, "atexit function 2 called\n");
}

static void onexitFunc(int exitStatus, void *arg) {
    fprintf(stdout, "on_exit function called: status=%d, arg=%ld\n", exitStatus, (long) arg);
}

int exitHandlers(int argc, char *argv[]) {
    if (on_exit(onexitFunc, (void *) 10) != 0) {
        fprintf(stderr, "on_exit 1\n");
        exit(EXIT_FAILURE);
    }

    if (atexit(atexitFunc1) != 0) {
        fprintf(stderr, "atexit 1\n");
        exit(EXIT_FAILURE);
    }

    if (atexit(atexitFunc2) != 0) {
        fprintf(stderr, "atexit 2\n");
        exit(EXIT_FAILURE);
    }

    if (on_exit(onexitFunc, (void *) 20) != 0) {
        fprintf(stderr, "atexit 2\n");
        exit(EXIT_FAILURE);
    }

    exit(2);
}