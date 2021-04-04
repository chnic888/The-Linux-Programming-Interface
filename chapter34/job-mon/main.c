#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

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


    return 0;
}