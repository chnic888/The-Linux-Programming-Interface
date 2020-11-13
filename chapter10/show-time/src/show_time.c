#include "show_time.h"
#include <time.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUF_SIZE 200

int showTime(int argc, char *argv[]) {
    time_t t;
    struct tm *loc;
    char buf[BUF_SIZE];

    if (setlocale(LC_ALL, "") == NULL) {
        fprintf(stderr, "failed to call setlocale, %d\n", errno);
        return (EXIT_FAILURE);
    }

    t = time(NULL);
    fprintf(stdout, "ctime() of time() value is: %s", ctime(&t));

    loc = localtime(&t);
    if (loc == NULL) {
        fprintf(stderr, "failed to call localtime(), %d\n", errno);
        return (EXIT_FAILURE);
    }
    fprintf(stdout, "asctime() of local time is: %s", asctime(loc));

    if (strftime(buf, BUF_SIZE, "%A, %d %B %Y, %H:%M:%S %Z", loc) == 0) {
        fprintf(stderr, "failed to call strftime(), %d\n", errno);
        return (EXIT_FAILURE);
    }
    fprintf(stdout, "strftime() of local time is: %s\n", buf);

    return (EXIT_SUCCESS);
}