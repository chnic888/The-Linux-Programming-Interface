#define _XOPEN_SOURCE

#include "strtime.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#include <errno.h>

int strtime(int argc, char *argv[]) {
    struct tm tm;
    char sbuf[SBUF_SIZE];
    char *ofmt;

    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s input-date-time in-format [out-format]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    if (setlocale(LC_ALL, "") == NULL) {
        fprintf(stderr, "failed to call setlocale(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    memset(&tm, 0, sizeof(struct tm));

    if (strptime(argv[1], argv[2], &tm) == NULL) {
        fprintf(stderr, "failed to call strptime(), %d", errno);
        return (EXIT_FAILURE);
    }

    //Not set by strptime(); tells mktime() to determine if DST is in effect
    tm.tm_isdst = -1;
    fprintf(stdout, "calendar time (seconds since Epoch): %ld\n", (long) mktime(&tm));

    ofmt = argc > 3 ? argv[3] : "%H:%M:%S %A, %d %B %Y %Z";
    if (strftime(sbuf, SBUF_SIZE, ofmt, &tm) == 0) {
        fprintf(stderr, "failed to call strftime(), %d", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "strftime() yields: %s\n", sbuf);

    return (EXIT_SUCCESS);
}