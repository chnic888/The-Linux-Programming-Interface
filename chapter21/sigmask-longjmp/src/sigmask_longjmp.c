#define _GNU_SOURCE

#include "sigmask_longjmp.h"
#include "signal_functions.h"

#include <stdlib.h>
#include <errno.h>
#include <setjmp.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t canJump = 0;

#ifdef USE_SIGSETJMP
static sigjmp_buf senv;
#else
static jmp_buf env;
#endif

static void handler(int sig) {
    fprintf(stdout, "Received signal %d (%s), signal mask is:\n", sig, strsignal(sig));
    printSigMask(stdout, NULL);

    if (!canJump) {
        printf("'env' buffer not yet set, doing a simple return\n");
        return;
    }

#ifdef USE_SIGSETJMP
    siglongjmp(senv, 1);
#else
    longjmp(env, 1);
#endif
}

int sigmaskLongjmp(int argc, char *argv[]) {
    struct sigaction sa;

    printSigMask(stdout, "Signal mask at startup:\n");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction(), %d\n", errno);
        return (EXIT_FAILURE);
    }

#ifdef USE_SIGSETJMP
    fprintf(stdout, "Calling sigsetjmp()\n");
    if (sigsetjmp(senv, 1) == 0)
#else
    fprintf(stdout, "Calling setjmp()\n");
    if (setjmp(env) == 0)
#endif
        canJump = 1;
    else
        printSigMask(stdout, "After jump from handler, signal mask is:\n");

    for (;;) {
        pause();
    }
}