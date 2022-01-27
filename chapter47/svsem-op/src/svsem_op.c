#include "svsem_op.h"
#include <sys/types.h>
#include <sys/sem.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_SEMOPS 1000

static void usageError(const char *progName) {
    fprintf(stderr, "Usage: %s semid op[,op...]...\n\n", progName);
    fprintf(stderr, "'op' is either: <sem#>{+|-}<value>[n][u]\n");
    fprintf(stderr, "            or: <sem#>=0[n]\n");
    fprintf(stderr, "            or: <sem#>=0[n]\n");

    exit(EXIT_FAILURE);
}

int svsemOp(int argc, char *argv[]) {
    return (EXIT_SUCCESS);
}