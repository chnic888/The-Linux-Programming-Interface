#include "calendar_time.h"
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <errno.h>

int calendarTime(int argc, char *argv[]) {
    time_t t;
    struct tm *gmp, *locp;
    struct tm gm, loc;
    struct timeval tv;

    t = time(NULL);
    fprintf(stdout, "Seconds since the Epoch (1 Jan 1970): %ld", (long) t);
    fprintf(stdout, "(about %6.3f years)\n", t / SECOND_IN_TROPICAL_YEAR);

    if (gettimeofday(&tv, NULL) == -1) {
        fprintf(stderr, "failed to call gettimeofday(), %d\n", errno);
        return (EXIT_FAILURE);
    }
    fprintf(stdout, "   gettimeofday() returned %ld secs, %ld microsecs\n", (long) tv.tv_sec, (long) tv.tv_usec);

    gmp = gmtime(&t);
    if (gmp == NULL) {
        fprintf(stderr, "failed to call gmtime(), %d\n", errno);
        return (EXIT_FAILURE);
    }
    gm = *gmp;
    fprintf(stdout, "Broken down by gmtime():\n");
    fprintf(stdout, "   year=%d mon=%d mday=%d hour=%d min=%d sec=%d ", gm.tm_year, gm.tm_mon, gm.tm_mday, gm.tm_hour,
            gm.tm_min, gm.tm_sec);
    fprintf(stdout, "wday=%d yday=%d isdst=%d\n", gm.tm_wday, gm.tm_yday, gm.tm_isdst);

    locp = localtime(&t);
    if (locp == NULL) {
        fprintf(stderr, "failed to call localtime(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    loc = *locp;
    fprintf(stdout, "Broken down by localtime():\n");
    fprintf(stdout, "   year=%d mon=%d mday=%d hour=%d min=%d sec=%d ", loc.tm_year, loc.tm_mon, loc.tm_mday,
            loc.tm_hour, loc.tm_min, loc.tm_isdst);

    fprintf(stdout, "asctime() formats the gmtime() value as : %s", asctime(&gm));
    fprintf(stdout, "ctime() formats the time() values as:     %s", ctime(&t));

    fprintf(stdout, "mktime() of gmtime() value:    %ld secs\n", (long) mktime(&gm));
    fprintf(stdout, "mktime() of localtime() value: %ld secs\n", (long) mktime(&loc));

    return (EXIT_SUCCESS);
}