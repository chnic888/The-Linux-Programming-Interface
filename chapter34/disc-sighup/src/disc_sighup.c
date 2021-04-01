#define _GNU_SOURCE

#include "disc_sighup.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>

static void handler(int);

int discSighup(int argc, char *argv[]) {
    pid_t parentPid, childPid;
    int j;
    struct sigaction sa;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s {d|s}... [ > sig.log 2>&1 ]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    parentPid = getpid();
    fprintf(stdout, "PID of parent process is:       %ld\n", (long) parentPid);
    fprintf(stdout, "Foreground process group ID is: %ld\n", (long) tcgetpgrp(STDIN_FILENO));

    for (j = 1; j < argc; j++) {
        childPid = fork();
        if (childPid == -1) {
            fprintf(stderr, "failed to call fork(), %d\n", errno);
            exit(EXIT_FAILURE);
        }

        if (childPid == 0) {
            if (argv[j][0] == 'd') {
                if (setpgid(0, 0) == -1) {
                    fprintf(stderr, "failed to call setpgid(), %d\n", errno);
                    exit(EXIT_FAILURE);
                }
            }

            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sa.sa_handler = handler;
            if (sigaction(SIGHUP, &sa, NULL) == -1) {
                fprintf(stderr, "failed to call sigaction(), %d\n", errno);
                exit(EXIT_FAILURE);
            }
            break;
        }
    }

    alarm(60);

    fprintf(stdout, "PID=%ld PGID=%ld\n", (long) getpid(), (long) getpgrp());
    for (;;) {
        pause();
    }
}

static void handler(int sig) {
    fprintf(stdout, "PID %ld: caught signal %2d (%s)\n", (long) getpid(), sig, strsignal(sig));
}