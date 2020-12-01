#include "t_umount.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <errno.h>

int tUmount(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s target\n", argv[0]);
        return (EXIT_FAILURE);
    }

    if (umount(argv[1]) == -1) {
        fprintf(stderr, "failed to call umount(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "umount %s successfully\n", argv[1]);
    return (EXIT_SUCCESS);
}