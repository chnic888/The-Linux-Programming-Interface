#include "fork_child_wait_parent.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#define SYNC_SIG SIGUSR1

static void handler(int sig) {

}

int forkChildWaitParent(int argc, char *argv[]) {
    pid_t childPid;
    sigset_t blockMask, origMask, emptyMask;
    struct sigaction sa;

    setbuf(stdout, NULL);

    switch (childPid = fork()) {
        case -1:
            fprintf(stderr, "failed to call fork(), %d\n", errno);
            exit(EXIT_FAILURE);
        case 0:
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sa.sa_handler = handler;
            if (sigaction(SYNC_SIG, &sa, NULL) == -1) {
                fprintf(stderr, "failed to call sigaction(), %d\n", errno);
                return (EXIT_FAILURE);
            }

            sigemptyset(&emptyMask);
            if (sigsuspend(&emptyMask) == -1 && errno != EINTR) {
                fprintf(stderr, "failed to call sigsuspend(), %d\n", errno);
                exit(EXIT_FAILURE);
            }
            fprintf(stdout, "[%ld] Child got signal\n", (long) getpid());

            fprintf(stdout, "child process quit\n");
            _exit(EXIT_SUCCESS);
        default:
            fprintf(stdout, "[%ld] Parent - doing some work\n", (long) getpid());
            sleep(2);

            fprintf(stdout, "[%ld] Parent about to signal child\n", (long) getpid());
            if (kill(childPid, SYNC_SIG) == -1) {
                fprintf(stderr, "failed to call kill(), %d\n", errno);
                exit(EXIT_FAILURE);
            }

            fprintf(stdout, "parent process quit\n");
            exit(EXIT_SUCCESS);
    }
}