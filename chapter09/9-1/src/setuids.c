#define _GNU_SOURCE

#include "setuids.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fsuid.h>
#include <string.h>

void showUIDs();

int runA(long);

int runB(long);

int runC(long);

int runD(long);

int runE(long);

int setuids(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "%s a/b/c/d/e uid\n", argv[0]);
        return (EXIT_FAILURE);
    }
    showUIDs();

    char *endptr = NULL;
    long uid = strtol(argv[2], &endptr, 10);
    int ret;

    switch (argv[1][0]) {
        case 'a':
            ret = runA(uid);
            break;
        case 'b':
            ret = runB(uid);
            break;
        case 'c':
            ret = runC(uid);
            break;
        case 'd':
            ret = runD(uid);
            break;
        case 'e':
            ret = runE(uid);
            break;
        default:
            fprintf(stderr, "invalid command\n");
            ret = -1;
    }

    return ret == -1 ? EXIT_FAILURE : EXIT_SUCCESS;
}

int runE(long uid) {
    fprintf(stdout, "==setresuid(-1, %d, %d)==\n", (int) uid, (int) (uid + 1000));
    if (setresuid(-1, uid, uid + 1000) == -1) {
        fprintf(stderr, "failed to setresuid\n");
        return (EXIT_FAILURE);
    }

    showUIDs();
    return (EXIT_SUCCESS);
}

int runD(long uid) {
    fprintf(stdout, "==setfsuid(%d)==\n", (int) uid);
    if (setfsuid(uid) == -1) {
        fprintf(stderr, "failed to setfsuid\n");
        return (EXIT_FAILURE);
    }

    showUIDs();
    return (EXIT_SUCCESS);
}

int runC(long uid) {
    fprintf(stdout, "==seteuid(%d)==\n", (int) uid);
    if (seteuid(uid) == -1) {
        fprintf(stderr, "failed to seteuid\n");
        return (EXIT_FAILURE);
    }

    showUIDs();
    return (EXIT_SUCCESS);
}

int runB(long uid) {
    fprintf(stdout, "==setreuid(–1, %d)==\n", (int) uid);
    if (setreuid(-1, uid) == -1) {
        fprintf(stderr, "failed to setreuid\n");
        return (EXIT_FAILURE);
    }

    showUIDs();
    return (EXIT_SUCCESS);
}

int runA(long uid) {
    fprintf(stdout, "== setuid(%d) ==\n", (int) uid);
    if (setuid(uid) == -1) {
        fprintf(stderr, "failed to setuid\n");
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