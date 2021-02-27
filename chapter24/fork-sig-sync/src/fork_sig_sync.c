#include "fork_sig_sync.h"
#include "curr_time.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#define SYNC_SIG SIGUSR1

static void handler(int sig) {

}

int forkSigSync(int argc, char *argv[]) {
    pid_t childPid;
    sigset_t blockMask, origMask, emptyMask;
    struct sigaction sa;

    setbuf(stdout, NULL);

    sigemptyset(&blockMask);
    sigaddset(&blockMask, SYNC_SIG);
    if (sigprocmask(SIG_BLOCK, &blockMask, &origMask) == -1) {
        fprintf(stderr, "failed to call sigprocmask(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SYNC_SIG, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    switch (childPid = fork()) {
        case -1:
            fprintf(stderr, "failed to call fork(), %d\n", errno);
            exit(EXIT_FAILURE);
        case 0:
            fprintf(stdout, "[%s %ld] Child started - doing some work\n", currTime("%T"), (long) getpid());
            sleep(2);
            fprintf(stdout, "[%s %ld] Child about to signal parent\n", currTime("%T"), (long) getpid());

            if (kill(getppid(), SYNC_SIG) == -1) {
                fprintf(stderr, "failed to call kill(), %d\n", errno);
                exit(EXIT_FAILURE);
            }

            fprintf(stdout, "child process quit\n");
            _exit(EXIT_SUCCESS);
        default:
            fprintf(stdout, "[%s %ld] Parent about to wait for signal\n", currTime("%T"), (long) getpid());
            sigemptyset(&emptyMask);
            if (sigsuspend(&emptyMask) == -1 && errno != EINTR) {
                fprintf(stderr, "failed to call sigsuspend(), %d\n", errno);
                exit(EXIT_FAILURE);
            }
            fprintf(stdout, "[%s %ld] Parent got signal\n", currTime("%T"), (long) getpid());

            if (sigprocmask(SIG_SETMASK, &origMask, NULL) == -1) {
                fprintf(stderr, "failed to call sigprocmask(), %d\n", errno);
                exit(EXIT_FAILURE);
            }

            fprintf(stdout, "parent process quit\n");
            exit(EXIT_SUCCESS);
    }
}