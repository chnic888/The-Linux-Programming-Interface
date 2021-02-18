#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

void handler(int sig) {
    fprintf(stdout, "Caught signal %d\n", sig);
}

int main(int argc, char *argv[]) {
    struct sigaction sa;
    char *endPtr;
    unsigned int remainSec;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction(), %d", errno);
        return (EXIT_FAILURE);
    }

    remainSec = sleep(strtol(argv[1], &endPtr, 10));
    fprintf(stdout, "remaining second %d\n", remainSec);

    return (EXIT_SUCCESS);
}