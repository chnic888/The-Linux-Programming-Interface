#define _POSIX_C_SOURCE 199309

#include "t_nanosleep.h"
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

static void sigHandler(int sig) {
    fprintf(stdout, "Caught a sig %d\n", sig);
}


int tNanosleep(int argc, char *argv[]) {
    struct timeval start, finish;
    struct timespec request, remain;
    struct sigaction sa;
    char *endPtr = NULL;
    int s;

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s secs nanosecs\n", argv[0]);
        return (EXIT_FAILURE);
    }

    request.tv_sec = strtol(argv[1], &endPtr, 10);
    endPtr = NULL;

    request.tv_nsec = strtol(argv[2], &endPtr, 10);
    endPtr = NULL;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigHandler;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (gettimeofday(&start, NULL) == -1) {
        fprintf(stderr, "failed to call gettimeofday(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    for (;;) {
        s = nanosleep(&request, &remain);
        if (s == -1 && errno != EINTR) {
            fprintf(stderr, "failed to call nanosleep(), %d\n", errno);
            exit(EXIT_FAILURE);
        }

        if (gettimeofday(&finish, NULL) == -1) {
            fprintf(stderr, "failed to call gettimeofday(), %d\n", errno);
            exit(EXIT_FAILURE);
        }
        fprintf(stdout, "Slept for: %9.6f secs\n",
                finish.tv_sec - start.tv_sec + (finish.tv_usec - start.tv_usec) / 1000000.0);

        if (s == 0) {
            break;
        }
        fprintf(stdout, "Remaining: %2ld.%09ld\n", remain.tv_sec, remain.tv_nsec);

        request = remain;
    }

    fprintf(stdout, "Sleep complete\n");
    exit(EXIT_SUCCESS);
}
