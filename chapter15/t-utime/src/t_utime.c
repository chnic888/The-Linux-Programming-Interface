#include "t_utime.h"
#include <sys/stat.h>
#include <utime.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int tUtime(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s filepath\n", argv[0]);
        return (EXIT_FAILURE);
    }

    struct stat sb;
    struct utimbuf utb;

    if (stat(argv[1], &sb) == -1) {
        fprintf(stderr, "failed to call stat(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    utb.actime = sb.st_atime;
    utb.modtime = sb.st_atime;

    if (utime(argv[1], &utb) == -1) {
        fprintf(stderr, "failed to call utime(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}