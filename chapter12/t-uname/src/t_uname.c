#include "t_uname.h"
#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int tUname(int argc, char *argv[]) {
    struct utsname uts;

    if (uname(&uts) == -1) {
        fprintf(stderr, "failed to call uname(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Node name:   %s\n", uts.nodename);
    fprintf(stdout, "System name: %s\n", uts.sysname);
    fprintf(stdout, "Release:     %s\n", uts.release);
    fprintf(stdout, "Version:     %s\n", uts.version);
    fprintf(stdout, "Machine:     %s\n", uts.machine);

    return (EXIT_SUCCESS);
}