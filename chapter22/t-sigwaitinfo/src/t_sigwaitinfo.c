#define _GNU_SOURCE

#include "t_sigwaitinfo.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int tSigwaitinfo(int argc, char *argv[]) {
    int sig;
    siginfo_t si;
    sigset_t allSigs;
    char *endPtr = NULL;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s [delay-secs]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "%s: PID is %ld\n", argv[0], (long) getpid());

    sigfillset(&allSigs);
    if (sigprocmask(SIG_SETMASK, &allSigs, NULL) == -1) {
        fprintf(stderr, "failed to call sigprocmask(), %d\n", errno);
        return (EXIT_FAILURE);
    }
    fprintf(stdout, "%s: signals blocked\n", argv[0]);

    if (argc > 1) {
        fprintf(stdout, "%s: about to delay %s seconds\n", argv[0], argv[1]);
        sleep(strtol(argv[1], &endPtr, 10));
        fprintf(stdout, "%s: finished delay\n", argv[0]);
    }

    for (;;) {
        sig = sigwaitinfo(&allSigs, &si);

        if (sig == -1) {
            fprintf(stderr, "failed to call sigwaitinfo(), %d\n", errno);
            exit(EXIT_FAILURE);
        }

        if (sig == SIGINT || sig == SIGTERM) {
            fprintf(stdout, "caught SIGINT or SIGTERM signal\n");
            exit(EXIT_SUCCESS);
        }

        fprintf(stdout, "got signal: %d (%s)\n", sig, strsignal(sig));
        printf(" si_signo=%d, si_code=%d (%s), si_value=%d\n", si.si_signo, si.si_code,
               (si.si_code == SI_USER) ? "SI_USER" : (si.si_code == SI_QUEUE) ? "SI_QUEUE" : "other",
               si.si_value.sival_int);
    }
}