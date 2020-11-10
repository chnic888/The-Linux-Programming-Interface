#include "curr_time.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int currTime(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s format\n", argv[0]);
        return (EXIT_FAILURE);
    }

    static char buf[BUF_SIZE];
    time_t t;
    struct tm *tm;

    t = time(NULL);
    tm = localtime(&t);
    if (tm == NULL) {
        fprintf(stderr, "failed to call localtime, %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (strftime(buf, BUF_SIZE, argv[1], tm) == 0) {
        fprintf(stderr, "failed to convert tm, %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "%s\n", buf);
    return (EXIT_SUCCESS);
}