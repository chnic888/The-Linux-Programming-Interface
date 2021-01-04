#define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

static void sigHandler(int sig) {
    fprintf(stdout, "Ouch!\n");
}

int main(int argc, char *argv[]) {
    int j;

    if (signal(SIGINT, sigHandler) == SIG_ERR) {
        fprintf(stderr, "signal failed, %d", errno);
        return (EXIT_FAILURE);
    }

    for (j = 0;; j++) {
        fprintf(stdout, "%d\n", j);
        sleep(3);
    }
}
