#include "curr_time.h"
#include <stdio.h>
#include <time.h>
#include <errno.h>

char *currTime(char *format) {
    static char buf[BUF_SIZE];
    time_t t;
    struct tm *tm;

    t = time(NULL);
    tm = localtime(&t);
    if (tm == NULL) {
        fprintf(stderr, "failed to call localtime, %d\n", errno);
        return NULL;
    }

    if (strftime(buf, BUF_SIZE, format, tm) == 0) {
        fprintf(stderr, "failed to convert tm, %d\n", errno);
        return NULL;
    }

    return buf;
}