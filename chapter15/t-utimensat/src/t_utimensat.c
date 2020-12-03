#include "t_utimensat.h"
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int tUtimensat(int argc, char *argv[]) {
    const char *optString = "a:m:";
    int opt;
    char *endPtr = NULL;
    struct timespec times[2] = {
            {.tv_sec = 0, .tv_nsec = UTIME_OMIT},
            {.tv_sec = 0, .tv_nsec = UTIME_OMIT},
    };

    while ((opt = getopt(argc, argv, optString)) != -1) {
        switch (opt) {
            case 'a':
                times[0].tv_sec = strtol(optarg, &endPtr, 10);
                times[0].tv_nsec = UTIME_NOW;
                endPtr = NULL;
                fprintf(stdout, "a=%ld\n", times[0].tv_sec);
                break;
            case 'm':
                times[1].tv_sec = strtol(optarg, &endPtr, 10);
                times[1].tv_nsec = UTIME_NOW;
                endPtr = NULL;
                fprintf(stdout, "m=%ld\n", times[1].tv_sec);
                break;
            default:
                break;
        }
    }

    if (argv[optind] == NULL) {
        fprintf(stderr, "file path is missing\n");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}