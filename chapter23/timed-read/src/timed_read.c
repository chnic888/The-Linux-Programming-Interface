#include "timed_read.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 200

static void handler(int sig) {
    fprintf(stdout, "Caught signal\n");
}

int timedRead(int argc, char *argv[]) {
    struct sigaction sa;
    char buf[BUF_SIZE];
    ssize_t numRead;
    int savedErrno;
    char *endPtr = NULL;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s [num-secs [restart-flag]]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    sa.sa_flags = (argc > 2) ? SA_RESTART : 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handler;
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    alarm((argc > 1) ? strtol(argv[1], &endPtr, 10) : 10);

    numRead = read(STDIN_FILENO, buf, BUF_SIZE - 1);

    savedErrno = errno;
    alarm(0);
    errno = savedErrno;

    if (numRead == -1) {
        if (errno == EINTR)
            fprintf(stderr, "Raed timed out\n");
        else
            fprintf(stderr, "read failed, %d\n", errno);
    } else {
        fprintf(stdout, "Successful read (%ld bytes): %.*s", (long) numRead, (int) numRead, buf);
    }

    return (EXIT_SUCCESS);
}