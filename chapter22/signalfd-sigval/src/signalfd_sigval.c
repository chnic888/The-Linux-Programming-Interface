#include "signalfd_sigval.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/signalfd.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int signalfdSigval(int argc, char *argv[]) {
    sigset_t mask;
    int sfd, j;
    struct signalfd_siginfo fdsi;
    ssize_t s;
    char *endPtr = NULL;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s sig-num...\n", argv[0]);
    }

    fprintf(stdout, "%s: PID = %ld\n", argv[0], (long) getpid());

    sigemptyset(&mask);
    for (j = 1; j < argc; j++) {
        sigaddset(&mask, (int) strtol(argv[j], &endPtr, 10));
        endPtr = NULL;
    }

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        fprintf(stderr, "failed to call sigprocmask(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    sfd = signalfd(-1, &mask, 0);
    if (sfd == -1) {
        fprintf(stderr, "failed to call signalfd(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    for (;;) {
        s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
        if (s != sizeof(struct signalfd_siginfo)) {
            fprintf(stderr, "read failed, %d\n", errno);
            exit(EXIT_FAILURE);
        }

        fprintf(stdout, "%s: got signal %d", argv[0], fdsi.ssi_signo);
        if (fdsi.ssi_code == SI_QUEUE) {
            fprintf(stdout, "; ssi_pid = %d; ", fdsi.ssi_pid);
            fprintf(stdout, "ssi_int = %d", fdsi.ssi_int);
        }

        fprintf(stdout, "\n");
    }
}