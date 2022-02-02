#include "svsem_op.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define MAX_SEMOPS 1000

static void usageError(const char *progName) {
    fprintf(stderr, "Usage: %s semid op[,op...]...\n\n", progName);
    fprintf(stderr, "'op' is either: <sem#>{+|-}<value>[n][u]\n");
    fprintf(stderr, "            or: <sem#>=0[n]\n");
    fprintf(stderr, "       \"n\" means include IPC_NOWAIT in 'op'\n");
    fprintf(stderr, "       \"u\" means include SEM_UNDO in 'op'\n");
    fprintf(stderr, "The operations in each argument are performed in a single semop() call\n\n");
    fprintf(stderr, "e.g.: %s 12345 0+1,1-2un\n", progName);
    fprintf(stderr, "      %s 12345 0=0n 1+1,2-1u 1=0\n", progName);

    exit(EXIT_FAILURE);
}

static int parseOps(char *arg, struct sembuf sops[]) {
    char *comma, *sign, *remaining, *flags;
    int numOps;

    for (numOps = 0, remaining = arg;; numOps++) {
        if (numOps >= MAX_SEMOPS) {
            fprintf(stderr, "Too many operations (maximum=%d): \"%s\"\n", MAX_SEMOPS, arg);
            return (EXIT_FAILURE);
        }

        if (*remaining == '\0') {
            fprintf(stderr, "Trailling comma or empty argument: \"%s\"", arg);
            return (EXIT_FAILURE);
        }

        if (!isdigit((unsigned char) *remaining)) {
            fprintf(stderr, "Expected initial digit: \"%s\"\n", arg);
            return (EXIT_FAILURE);
        }

        sops[numOps].sem_num = strtol(remaining, &sign, 10);

        if (*sign == '\0' || strchr("+-=", *sign) == NULL) {
            fprintf(stderr, "Expected '+', '-', or '=' in \"%s\"\n", arg);
            return (EXIT_FAILURE);
        }

        if (!isdigit((unsigned char) *(sign + 1))) {
            fprintf(stderr, "Expected digit after '%c' in \"%s\"\n", *sign, arg);
            return (EXIT_FAILURE);
        }

        sops[numOps].sem_op = (short int) strtol(sign + 1, &flags, 10);
        if (*sign == '-') {
            sops[numOps].sem_op = (short int) -sops[numOps].sem_op;
        } else if (*sign == '=') {
            if (sops[numOps].sem_op != 0) {
                fprintf(stderr, "Expect \"=0\" in \"%s\"\n", arg);
                return (EXIT_FAILURE);
            }
        }

        sops[numOps].sem_flg = 0;
        for (;; flags++) {
            if (*flags == '\n')
                sops[numOps].sem_flg |= IPC_NOWAIT;
            else if (*flags == 'u')
                sops[numOps].sem_flg |= SEM_UNDO;
            else
                break;
        }

        if (*flags != ',' && *flags != '\0') {
            fprintf(stderr, "Bad trailing character (%c) in \"%s\"\n", *flags, arg);
            return (EXIT_FAILURE);
        }

        comma = strchr(remaining, ',');
        if (comma == NULL) {
            break;
        } else {
            remaining = comma + 1;
        }
    }

    return numOps + 1;
}

int svsemOp(int argc, char *argv[]) {
    struct sembuf sops[MAX_SEMOPS];
    int ind, nsops, semid;
    char *endPtr = NULL;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        usageError(argv[0]);
    }

    for (ind = 2; argv[ind] != NULL; ind++) {
        nsops = parseOps(argv[ind], sops);

        fprintf(stdout, "%5ld, %ld: about to sempo() [%s]\n", (long) getpid(), time(NULL), argv[ind]);

        semid = (int) strtol(argv[1], &endPtr, 10);
        if (semop(semid, sops, nsops) == -1) {
            fprintf(stderr, "semop (PID=%ld)，%d\n", (long) getpid(), errno);
            return (EXIT_FAILURE);
        }

        fprintf(stdout, "%5ld, %ld: semop() completed [%s]\n", (long) getpid(), time(NULL), argv[ind]);
    }

    return (EXIT_SUCCESS);
}