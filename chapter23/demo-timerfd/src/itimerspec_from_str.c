#define _POSIX_C_SOURCE 199309

#include "itimerspec_from_str.h"
#include <string.h>
#include <stdlib.h>

void itimerspecFromStr(char *str, struct itimerspec *tsp) {
    char *cptr, *sptr;
    char *endPtr = NULL;

    cptr = strchr(str, ':');
    if (cptr != NULL) {
        *cptr = '\0';
    }

    sptr = strchr(str, '/');
    if (sptr != NULL) {
        *sptr = '\0';
    }

    tsp->it_value.tv_sec = strtol(str, &endPtr, 10);
    endPtr = NULL;

    tsp->it_value.tv_nsec = (sptr != NULL) ? strtol(sptr + 1, &endPtr, 10) : 0;
    endPtr = NULL;

    if (cptr == NULL) {
        tsp->it_interval.tv_sec = 0;
        tsp->it_interval.tv_nsec = 0;
    } else {
        sptr = strchr(cptr + 1, '/');
        if (sptr != NULL) {
            *sptr = '\0';
        }

        tsp->it_interval.tv_sec = strtol(cptr + 1, &endPtr, 10);
        endPtr = NULL;

        tsp->it_interval.tv_nsec = (sptr != NULL) ? strtol(cptr + 1, &endPtr, 10) : 0;
        endPtr = NULL;
    }
}

