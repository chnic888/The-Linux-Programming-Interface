#define _GNU_SOURCE

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

static void handler(int sig) {
    fprintf(stdout, "PID=%ld: caught signal %d (%s)\n", (long) getpid(), sig, strsignal(sig));
}

int main(int argc, char *argv[]) {
    int j;
    struct sigaction sa;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s {s|p} ...\n", argv[0]);
        return (EXIT_FAILURE);
    }

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if (sigaction(SIGHUP, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGCONT, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "parent: PID=%ld, PPID=%ld, PGID=%ld, SID=%ld\n", (long) getpid(), (long) getppid(),
            (long) getpgrp(), (long) getsid(0));

    for (j = 1; j < argc; j++) {
        switch (fork()) {
            case -1:
                fprintf(stderr, "failed to call fork(), %d\n", errno);
                exit(EXIT_FAILURE);
            case 0:
                fprintf(stdout, "child: PID=%ld, PPID=%ld, PGID=%ld, SID=%ld\n", (long) getpid(), (long) getppid(),
                        (long) getpgrp(), (long) getsid(0));
                if (argv[j][0] == 's') {
                    fprintf(stdout, "PID=%ld stopping\n", (long) getpid());
                    raise(SIGSTOP);
                } else {
                    alarm(10);
                    fprintf(stdout, "PID=%ld pausing\n", (long) getpid());
                    pause();
                }
                _exit(EXIT_SUCCESS);
            default:
                break;
        }
    }

    sleep(3);
    fprintf(stdout, "parent exiting\n");
    exit(EXIT_SUCCESS);
}
