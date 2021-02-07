#define _GNU_SOURCE

#include "t_sigaltstack.h"
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

static void sigsegvHandler(int sig) {
    int x;

    fprintf(stdout, "Caught signal %d (%s)\n", sig, strsignal(sig));
    fprintf(stdout, "Top of handler near     %10p\n", (void *) &x);
    fflush(NULL);

    _exit(EXIT_FAILURE);
}

static void overflowStack(int callNum) {
    char a[10000];

    fprintf(stdout, "Call %4d - top of stack near %10p\n", callNum, &a[0]);
    overflowStack(callNum + 1);
}

int tSigaltstack(int argc, char *argv[]) {
    stack_t sigstack;
    struct sigaction sa;
    int j;

    fprintf(stdout, "Top of standard stack is near %10p\n", (void *) &j);

    sigstack.ss_sp = malloc(SIGSTKSZ);
    if (sigstack.ss_sp == NULL) {
        fprintf(stdout, "failed to malloc memory, %d\n", errno);
        return (EXIT_FAILURE);
    }

    sigstack.ss_size = SIGSTKSZ;
    sigstack.ss_flags = 0;
    if (sigaltstack(&sigstack, NULL) == -1) {
        fprintf(stdout, "failed to call sigaltstack(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Alternate stack is at %10p-%p\n", sigstack.ss_sp, (char *) sbrk(0) - 1);
    sa.sa_handler = sigsegvHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_ONSTACK;  //makes handler use the alternate stack

    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction(), %d\n", errno);
    }

    overflowStack(1);
}
