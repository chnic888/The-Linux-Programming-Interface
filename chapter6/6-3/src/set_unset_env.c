#include "set_unset_env.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int setEnvironment(const char *);

static int unsetEnvironment(const char *);

int setUnsetEnv(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "%s set/unset k=v/k", argv[0]);
        return (EXIT_FAILURE);
    }

    if (strcmp(argv[1], "set") == 0) {

    } else {

    }

    return (EXIT_SUCCESS);
}

static int setEnvironment(const char *env) {
    return 0;
}

static int unsetEnvironment(const char *envName) {
    return 0;
}

