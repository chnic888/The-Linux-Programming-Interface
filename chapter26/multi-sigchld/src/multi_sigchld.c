#include "multi_sigchld.h"
#include "print_wait_status.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

static volatile int numLiveChildren = 0;

static void sigchldHandler(int sig) {
    int status, savedErrno;
    pid_t childPid;

    savedErrno = errno;
    fprintf(stdout, "%ld handler: Caught SIGCHLD\n", time(NULL));

    while ((childPid = waitpid(-1, &status, WNOHANG)) > 0) {
        fprintf(stdout, "%ld handler: Reaped child %ld - ", time(NULL), (long) childPid);
        printWaitStatus(NULL, status);
        numLiveChildren--;
    }

    if (childPid == -1 && errno != ECHILD) {
        fprintf(stderr, "failed to call waitpid(), %d\n", errno);
        _exit(EXIT_FAILURE);
    }

    sleep(5);
    fprintf(stdout, "%ld handler: returning\n", time(NULL));

    errno = savedErrno;
}

int multiSigchld(int argc, char *argv[]) {
    int j, sigCnt;
    sigset_t blockMask, emptyMask;
    struct sigaction sa;
    char *endPtr = NULL;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s child-sleep-time...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    sigCnt = 0;
    numLiveChildren = argc - 1;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigchldHandler;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGCHLD);
    if (sigprocmask(SIG_SETMASK, &blockMask, NULL) == -1) {
        fprintf(stderr, "failed to call sigprocmask(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    for (j = 1; j < argc; j++) {
        switch (fork()) {
            case -1:
                fprintf(stderr, "failed to call fork(), %d\n", errno);
                exit(EXIT_FAILURE);
            case 0:
                sleep(strtol(argv[j], &endPtr, 10));
                endPtr = NULL;
                fprintf(stdout, "%ld Child %d (PID=%ld) exiting\n", time(NULL), j, (long) getpid());
                _exit(EXIT_SUCCESS);
            default:
                break;
        }
    }

    sigemptyset(&emptyMask);
    while (numLiveChildren > 0) {
        fprintf(stdout, "before sigsuspend(), numLiveChildren = %d\n", numLiveChildren);
        if (sigsuspend(&emptyMask) == -1 && errno != EINTR) {
            fprintf(stderr, "failed to call sigsuspend(), %d\n", errno);
            exit(EXIT_FAILURE);
        }
        fprintf(stdout, "after sigsuspend(), numLiveChildren = %d\n", numLiveChildren);
        sigCnt++;
    }

    fprintf(stdout, "%ld All %d children have terminated; SIGCHLD was caught %d times\n", time(NULL), argc - 1, sigCnt);

    exit(EXIT_SUCCESS);
}