#include "real_time.h"
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

static volatile sig_atomic_t gotAlarm = 0;

static void displayTimes(const char *msg, bool includeTimer) {
    struct itimerval itv;
    static struct timeval start;
    struct timeval curr;
    static int callNum = 0;

    if (callNum == 0) {
        if (gettimeofday(&start, NULL) == -1) {
            fprintf(stderr, "failed to call gettimeofday(), %d\n", errno);
            _exit(EXIT_FAILURE);
        }
    }

    if (callNum % 20 == 0) {
        fprintf(stdout, "    Elapsed Value Interval\n");
    }

    if (gettimeofday(&curr, NULL) == -1) {
        fprintf(stderr, "failed to call gettimeofday(), %d\n", errno);
        _exit(EXIT_FAILURE);
    }
    fprintf(stdout, "%-7s %6.2f", msg, curr.tv_sec - start.tv_sec + (curr.tv_usec - start.tv_usec) / 1000000.0);

    if (includeTimer) {
        if (getitimer(ITIMER_REAL, &itv) == -1) {
            fprintf(stderr, "failed to call getitimer(), %d\n", errno);
            _exit(EXIT_FAILURE);
        }

        fprintf(stdout, " %6.2f %6.2f", itv.it_value.tv_sec + itv.it_value.tv_usec / 1000000.0,
                itv.it_interval.tv_sec + itv.it_interval.tv_usec / 1000000.0);
    }

    fprintf(stdout, "\n");
    callNum++;
}

static void sigalrmHandler(int sig) {
    gotAlarm = 1;
}

int realTime(int argc, char *argv[]) {
    struct itimerval itv;
    clock_t prevClock;
    int maxSigs;
    int sigCnt;
    struct sigaction sa;
    char *endPtr = NULL;

    if (argc> 1 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s [secs [usecs [int-secs [int-usecs]]]]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    sigCnt = 0;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigalrmHandler;
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    maxSigs = (itv.it_interval.tv_sec == 0 && itv.it_interval.tv_usec == 0) ? 1 : 3;
    displayTimes("START:", false);

    itv.it_value.tv_sec = (argc > 1) ? strtol(argv[1], &endPtr, 10) : 2;
    endPtr = NULL;

    itv.it_value.tv_usec = (argc > 2) ? strtol(argv[2], &endPtr, 10) : 0;
    endPtr = NULL;

    itv.it_interval.tv_sec = (argc > 3) ? strtol(argv[3], &endPtr, 10) : 0;
    endPtr = NULL;

    itv.it_interval.tv_usec = (argc > 4) ? strtol(argv[4], &endPtr, 10) : 0;
    endPtr = NULL;

    if(setitimer(ITIMER_REAL, &itv, 0) == -1) {
        fprintf(stderr, "failed to call setitimer(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    prevClock = clock();
    sigCnt = 0;

    for(;;) {
        while (((clock() - prevClock) * 10 / CLOCKS_PER_SEC) < 5) {
            if (gotAlarm) {
                gotAlarm = 0;
                displayTimes("ALARM:", true);

                sigCnt++;
                if (sigCnt >= maxSigs) {
                    fprintf(stdout, "That's all folks\n");
                    exit(EXIT_SUCCESS);
                }
            }
        }

        prevClock = clock();
        displayTimes("Main: ", true);
    }
}