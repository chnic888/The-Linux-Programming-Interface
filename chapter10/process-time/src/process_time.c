#include "process_time.h"
#include <sys/times.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

static void displayProcessTime(const char *);

int processTime(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s num-calls\n", argv[0]);
        return (EXIT_FAILURE);
    }

    int numCalls, j;
    char *endPtr;

    fprintf(stdout, "CLOCKS_PER_SEC=%ld sysconf(_SC_CLK_TCK)=%ld\n\n", (long) CLOCKS_PER_SEC, sysconf(_SC_CLK_TCK));
    displayProcessTime("At program start:\n");

    numCalls = (int) strtol(argv[1], &endPtr, 10);
    for (j = 0; j < numCalls; j++) {
        (void) getppid();
    }

    displayProcessTime("After getppid() loop:\n");
    return (EXIT_SUCCESS);
}

static void displayProcessTime(const char *msg) {
    struct tms t;
    clock_t clockTime;
    static long clockTicks = 0;

    if (msg != NULL) {
        fprintf(stdout, "%s", msg);
    }

    //Fetch clock ticks on first call
    if (clockTicks == 0) {
        clockTicks = sysconf(_SC_CLK_TCK);
        if (clockTicks == -1) {
            fprintf(stderr, "failed to call sysconf(), %d\n", errno);
            return;
        }
    }

    clockTime = clock();
    if (clockTime == -1) {
        fprintf(stderr, "failed to call clock(), %d\n", errno);
        return;
    }
    fprintf(stdout, "   clock() returns: %ld clocks-per-sec (%.2f secs)\n", (long) clockTime,
            (double) clockTime / CLOCKS_PER_SEC);

    if (times(&t) == -1) {
        fprintf(stderr, "failed to call times(), %d\n", errno);
        return;
    }
    fprintf(stdout, "   times() yields: user CPU=%.2f; system CPU: %.2f\n", (double) t.tms_utime / clockTicks,
            (double) t.tms_stime / clockTicks);
}