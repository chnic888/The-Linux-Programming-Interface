#include "t_fpathconf.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

static void fpathconfPrint(const char *, int, int);

int tFpathconf(int argc, char *argv[]) {
    fpathconfPrint("_PC_NAME_MAX: ", STDIN_FILENO, _PC_NAME_MAX);
    fpathconfPrint("_PC_PATH_MAX: ", STDIN_FILENO, _PC_PATH_MAX);
    fpathconfPrint("_PC_PIPE_BUF: ", STDIN_FILENO, _PC_PIPE_BUF);

    return (EXIT_SUCCESS);
}

static void fpathconfPrint(const char *msg, int fd, int name) {
    long lim;

    errno = 0;
    lim = fpathconf(fd, name);

    if (lim != -1) {
        fprintf(stdout, "%s %ld\n", msg, lim);
        return;
    }

    if (errno == 0) {
        fprintf(stdout, "%s (indeterminate)\n", msg);
        return;
    }

    fprintf(stderr, "fpathconf %s\n", msg);
}