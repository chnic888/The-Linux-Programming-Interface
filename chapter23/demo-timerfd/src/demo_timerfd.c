#include "itimerspec_from_str.h"
#include "demo_timerfd.h"
#include <sys/timerfd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int demoTimerfd(int argc, char *argv[]) {
    struct itimerspec ts;
    struct timespec start, now;
    int maxExp, fd, secs, nanosecs;
    uint64_t numExp, totalExp;
    ssize_t s;
    char *endPtr;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s secs[/nsecs][:int-secs[/int-nsecs]] [max-exp]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    itimerspecFromStr(argv[1], &ts);
    maxExp = (argc > 2) ? (int) strtol(argv[2], &endPtr, 10) : 1;
    fd = timerfd_create(CLOCK_REALTIME, 0);
    if (fd == -1) {
        fprintf(stderr, "failed to call timerfd_create(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (timerfd_settime(fd, 0, &ts, NULL) == -1) {
        fprintf(stderr, "failed to call timerfd_settime(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) {
        fprintf(stderr, "failed to call clock_gettimee(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    for (totalExp = 0; totalExp < maxExp;) {
        s = read(fd, &numExp, sizeof(uint64_t));
        if (s != sizeof(uint64_t)) {
            fprintf(stderr, "failed to call read(), %d\n", errno);
            exit(EXIT_FAILURE);
        }

        totalExp += numExp;

        if (clock_gettime(CLOCK_MONOTONIC, &now) == -1) {
            fprintf(stderr, "failed to call clock_gettime(), %d\n", errno);
            exit(EXIT_FAILURE);
        }

        secs = now.tv_sec - start.tv_sec;
        nanosecs = now.tv_nsec - start.tv_nsec;
        if (nanosecs < 0) {
            secs--;
            nanosecs += 1000000000;
        }

        fprintf(stdout, "%d.%03d: expirations read: %llu; total=%llu\n", secs, (nanosecs + 500000) / 1000000,
                (unsigned long long) numExp, (unsigned long long) totalExp);
    }

    if (close(fd) == -1) {
        fprintf(stderr, "failed to call close(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}