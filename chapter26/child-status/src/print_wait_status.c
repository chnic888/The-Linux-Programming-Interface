#define _GNU_SOURCE

#include "print_wait_status.h"
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>

void printWaitStatus(const char *msg, int status) {
    if (msg != NULL) {
        fprintf(stdout, "%s", msg);
    }

    if (WIFEXITED(status)) {
        fprintf(stdout, "child exited, status=%d\n", WEXITSTATUS(status));
    } else if (WEXITSTATUS(status)) {
        fprintf(stdout, "child killed by signal %d (%s)", WTERMSIG(status), strsignal(WTERMSIG(status)));
#ifdef WCOREDUMP
        if (WCOREDUMP(status)) {
            fprintf(stdout, " (core dumped)");
        }
#endif
        fprintf(stdout, "\n");
    } else if (WIFSTOPPED(status)) {
        fprintf(stdout, "child stopped by signal %d (%s)\n", WSTOPSIG(status), strsignal(WSTOPSIG(status)));
#ifdef WIFCONTINUED
    } else if (WIFCONTINUED(status)) {
        fprintf(stdout, "child continued\n");
#endif
    } else {
        fprintf(stdout, "what happened to this child? (status=%x)\n", (unsigned int) status);
    }
}
