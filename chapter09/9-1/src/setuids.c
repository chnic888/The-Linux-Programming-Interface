#define _GNU_SOURCE

#include "setuids.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fsuid.h>

void showUIDs();

int setuids(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s uid\n", argv[0]);
        return (EXIT_FAILURE);
    }
    showUIDs();

    char *endptr = NULL;
    long uid = strtol(argv[1], &endptr, 10);

    fprintf(stdout, "== setuid(%d) ==\n", (int) uid);
    if (setuid(uid) == -1) {
        fprintf(stderr, "failed to setuid\n");
        return (EXIT_FAILURE);
    }
    showUIDs();

    fprintf(stdout, "==setreuid(–1, %d)==\n", (int) uid);
    if (setreuid(-1, uid) == -1) {
        fprintf(stderr, "failed to setreuid\n");
        return (EXIT_FAILURE);
    }
    showUIDs();

    fprintf(stdout, "==seteuid(%d)==\n", (int) uid);
    if (seteuid(uid) == -1) {
        fprintf(stderr, "failed to seteuid\n");
        return (EXIT_FAILURE);
    }
    showUIDs();

    return (EXIT_SUCCESS);
}

void showUIDs() {
    uid_t ruid, euid, suid, fsuid;

    if (getresuid(&ruid, &euid, &suid) == -1) {
        fprintf(stderr, "failed to getresuid");
        return;
    }

    fprintf(stdout, "####show uid####\n");
    fsuid = setfsuid(0);
    fprintf(stdout, "real uid = %d\n", ruid);
    fprintf(stdout, "effective uid = %d\n", euid);
    fprintf(stdout, "saved set-user-ID = %d\n", suid);
    fprintf(stdout, "fs id = %d\n", fsuid);
    fprintf(stdout, "\n");
}