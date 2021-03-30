#define _XOPEN_SOURCE 500

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (fork() != 0) {
        _exit(EXIT_SUCCESS);
    }

    if (setsid() == -1) {
        fprintf(stderr, "failed to call setsid(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "PID=%ld, PGID=%ld, SID=%ld\n", (long) getpid(), (long) getpgrp(), (long) getsid(0));

    if (open("/dev/tty", O_RDWR) == -1) {
        fprintf(stderr, "failed to open /dev/tty, %d\n", errno);
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}