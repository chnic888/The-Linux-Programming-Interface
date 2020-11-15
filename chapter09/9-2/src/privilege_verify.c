#define _GNU_SOURCE

#include "privilege_verify.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/fsuid.h>
#include <errno.h>

static void showUIDs();

int privilegeVerify(int argc, char *argv[]) {
    showUIDs();
    return (EXIT_SUCCESS);
}

static void showUIDs() {
    uid_t ruid, euid, suid, fsuid;
    if (getresuid(&ruid, &euid, &suid) == -1) {
        fprintf(stderr, "failde to call getresuid(), %d\n", errno);
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