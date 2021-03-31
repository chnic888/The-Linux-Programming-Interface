#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>

static void handler(int sig) {

}

int main(int argc, char *argv[]) {
    pid_t childPid;
    struct sigaction sa;

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGHUP, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    childPid = fork();
    if (childPid == -1) {
        fprintf(stderr, "failed to call fork(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if (childPid == 0 && argc > 1) {
        if (setpgid(0, 0) == -1) {
            fprintf(stderr, "failed to call setpgid(), %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }

    fprintf(stdout, "PID=%ld; PPID=%ld; PGID=%ld; SID=%ld\n", (long) getpid(), (long) getppid(), (long) getpgrp(),
            (long) getsid(0));

    alarm(60);

    for (;;) {
        pause();
        fprintf(stdout, "%ld: caught SIGHUP\n", (long) getpid());
    }
}