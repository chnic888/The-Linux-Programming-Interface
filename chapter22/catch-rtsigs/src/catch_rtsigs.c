#define _GNU_SOURCE

#include "catch_rtsigs.h"
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static volatile int handlerSleepTime;
static volatile int sigCnt = 0;
static volatile int allDone = 0;

static void siginfoHandler(int sig, siginfo_t *si, void *ucontext) {
    if (sig == SIGINT || sig == SIGTERM) {
        allDone = 1;
        return;
    }

    sigCnt++;
    fprintf(stdout, "caught signal %d\n", sig);

    fprintf(stdout, "   si_signo=%d, si_code=%d (%s),", si->si_signo, si->si_code,
            (si->si_code == SI_USER) ? "SI_USER" : (si->si_code == SI_QUEUE) ? "SI_QUEUE" : "other");
    fprintf(stdout, "si_value=%d\n", si->si_value.sival_int);
    fprintf(stdout, "   si_pid=%ld, si_uid=%ld\n", (long) si->si_pid, (long) si->si_uid);

    sleep(handlerSleepTime);
}

int catchRtsigs(int argc, char *argv[]) {
    struct sigaction sa;
    int sig;
    sigset_t prevMask, blockMask;
    char *endPtr = NULL;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s [block-time [handler-sleep-time]]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "%s: PID is %ld\n", argv[0], (long) getpid());

    handlerSleepTime = argc > 2 ? (int) strtol(argv[2], &endPtr, 10) : 1;
    endPtr = NULL;

    sa.sa_sigaction = siginfoHandler;
    sa.sa_flags = SA_SIGINFO;
    sigfillset(&sa.sa_mask);

    for (sig = 1; sig < NSIG; sig++) {
        if (sig != SIGTSTP && sig != SIGQUIT) {
            sigaction(sig, &sa, NULL);
        }
    }

    if (argc > 1) {
        sigfillset(&blockMask);
        sigdelset(&blockMask, SIGINT);
        sigdelset(&blockMask, SIGTERM);

        if (sigprocmask(SIG_SETMASK, &blockMask, &prevMask) == -1) {
            fprintf(stderr, "failed to call sigprocmask(), %d\n", errno);
            exit(EXIT_FAILURE);
        }

        fprintf(stdout, "%s: signals blocked - sleeping %s seconds\n", argv[0], argv[1]);
        sleep(strtol(argv[1], &endPtr, 10));
        fprintf(stdout, "%s: sleep complete\n", argv[0]);

        if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1) {
            fprintf(stderr, "failed to call sigprocmask(), %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }

    while (!allDone)
        pause();

    return (EXIT_SUCCESS);
}