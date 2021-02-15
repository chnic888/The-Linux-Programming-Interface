#define _GNU_SOURCE

#include "t_sigsuspend.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <signal.h>

static void printSigset(FILE *of, const char *, const sigset_t *);

static int printSigMask(FILE *, const char *);

static int printPendingSigs(FILE *, const char *);

static volatile sig_atomic_t gotSigquit = 0;

static void handler(int sig) {
    fprintf(stdout, "Caught signal %d (%s)\n", sig, strsignal(sig));

    if (sig == SIGQUIT)
        gotSigquit = 1;
}

int tSigsuspend(int argc, char *argv[]) {
    int loopNum;
    time_t startTime;
    sigset_t origMask, blockMask;
    struct sigaction sa;

    printSigMask(stdout, "Initial signal mask is:\n");

    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGINT);
    sigaddset(&blockMask, SIGQUIT);
    if (sigprocmask(SIG_BLOCK, &blockMask, &origMask) == -1) {
        fprintf(stderr, "failed to call sigprocmask(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (sigaction(SIGQUIT, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    for (loopNum = 1; !gotSigquit; loopNum++) {
        fprintf(stdout, "=== LOOP %d\n", loopNum);

        printSigMask(stdout, "Starting critical section, signal mask is:\n");
        for (startTime = time(NULL); time(NULL) < startTime + 10;)
            continue;

        printPendingSigs(stdout, "Before sigsuspend() - pending signals:\n");
        printSigset(stdout, "origMask\n", &origMask);

        if (sigsuspend(&origMask) == -1 && errno != EINTR) {
            fprintf(stderr, "failed to call sigpending(), %d\n", errno);
            return (EXIT_FAILURE);
        }
        printSigMask(stdout, "After sigsuspend() signal mask is:\n");
    }

    if (sigprocmask(SIG_SETMASK, &origMask, NULL) == -1) {
        fprintf(stderr, "sigprocmask - SIG_SETMASK, %d\n", errno);
        return (EXIT_FAILURE);
    }

    printSigMask(stdout, "=== Exited loop\nRestored signal mask to:\n");
    return (EXIT_SUCCESS);
}

void printSigset(FILE *of, const char *prefix, const sigset_t *sigset) {
    int sig, cnt;

    cnt = 0;
    for (sig = 1; sig < NSIG; sig++) {
        if (sigismember(sigset, sig)) {
            cnt++;
            fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
        }
    }

    if (cnt == 0)
        fprintf(of, "%s<empty signal set>\n", prefix);
}

static int printSigMask(FILE *of, const char *msg) {
    sigset_t currMask;

    if (msg != NULL)
        fprintf(of, "%s", msg);

    if (sigprocmask(SIG_BLOCK, NULL, &currMask) == -1) {
        fprintf(stderr, "failed to call sigprocmask(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    printSigset(of, "\t\t", &currMask);
    return (EXIT_SUCCESS);
}

static int printPendingSigs(FILE *of, const char *msg) {
    sigset_t pendingSets;

    if (msg != NULL)
        fprintf(of, "%s", msg);

    if (sigpending(&pendingSets) == -1) {
        fprintf(stderr, "failed to call sigpending(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    printSigset(of, "\t\t", &pendingSets);
    return (EXIT_SUCCESS);
}

