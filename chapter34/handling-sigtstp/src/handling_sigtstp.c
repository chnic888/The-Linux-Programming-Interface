#include "handling_sigtstp.h"
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void tstpHandler(int sig) {
    sigset_t tstpMask, prevMask;
    int savedErrno;
    struct sigaction sa;

    savedErrno = errno;

    fprintf(stdout, "Caught SIGTSTP\n");

    if (signal(SIGTSTP, SIG_DFL) == SIG_ERR) {
        fprintf(stderr, "failed to call signal(), %d\n", errno);
        return;
    }

    raise(SIGTSTP);

    sigemptyset(&tstpMask);
    sigaddset(&tstpMask, SIGTSTP);
    if (sigprocmask(SIG_UNBLOCK, &tstpMask, &prevMask) == -1) {
        fprintf(stderr, "failed to call sigprocmask(), %d\n", errno);
        return;
    }

    if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1) {
        fprintf(stderr, "failed to call sigprocmask(), %d\n", errno);
        return;
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = tstpHandler;
    if (sigaction(SIGTSTP, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction(), %d\n", errno);
        return;
    }

    fprintf(stdout, "Exiting SIGTSTP handler\n");
    errno = savedErrno;
}

int handlingSigtstp(int argc, char *argv[]) {
    struct sigaction sa;

    if (sigaction(SIGTSTP, NULL, &sa) == -1) {
        fprintf(stderr, "failed to call sigaction(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if (sa.sa_handler != SIG_IGN) {
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sa.sa_handler = tstpHandler;
        if (sigaction(SIGTSTP, &sa, NULL) == -1) {
            fprintf(stderr, "failed to call sigaction(), %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }

    for (;;) {
        pause();
        fprintf(stdout, "Main\n");
    }
}