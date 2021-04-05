#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

static int cmdNum;

static void handler(int sig) {
    if (getpid() == getpgrp()) {
        fprintf(stderr, "Terminal FG process group: %ld\n", (long) tcgetpgrp(STDERR_FILENO));
    }

    fprintf(stderr, "Process %ld (%d) received signal %d (%s)\n", (long) getpid(), cmdNum, sig, strsignal(sig));

    if (sig == SIGTSTP) {
        raise(SIGSTOP);
    }
}

int main(int argc, char *argv[]) {
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction() with SIGINT, %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGTSTP, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction() with SIGTSTP, %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGCONT, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction() with SIGCONT, %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if (isatty(STDIN_FILENO)) {
        fprintf(stderr, "Terminal FG process group: %ld\n", (long) tcgetpgrp(STDIN_FILENO));
        fprintf(stderr, "Command    PID    PPID    PGRP    SID\n");
        cmdNum = 0;
    } else {
        if (read(STDIN_FILENO, &cmdNum, sizeof(cmdNum)) <= 0) {
            fprintf(stderr, "read got EOF or error\n");
            exit(EXIT_FAILURE);
        }
    }

    cmdNum++;

    fprintf(stderr, "%4d    %5ld    %5ld    %5ld    %5ld\n", cmdNum, (long) getpid(), (long) getppid(), (long) getpgrp(),
            (long) getsid(0));

    if (!isatty(STDOUT_FILENO)) {
        if (write(STDOUT_FILENO, &cmdNum, sizeof(cmdNum)) == -1) {
            fprintf(stderr, "failed to write\n");
        }
    }

    for (;;) {
        pause();
    }
}