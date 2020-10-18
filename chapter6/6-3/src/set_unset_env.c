#include "set_unset_env.h"
#include <string.h>
#include <errno.h>

extern char **environ;

static int setEnvironment(const char *, long);

static int unsetEnvironment(const char *);

int setUnsetEnv(int argc, char *argv[]) {
    if (argc != 3 && argc != 4) {
        fprintf(stderr, "%s set/unset k=v/k [overwrite]", argv[0]);
        return (EXIT_FAILURE);
    }

    if (strcmp(argv[1], "set") == 0) {
        char *end = argv[3] + strlen(argv[3]);
        long overwrite = strtol(argv[3], &end, 0);

        return setEnvironment(argv[2], overwrite);
    }

    if (strcmp(argv[1], "unset") == 0) {
        return unsetEnvironment(argv[2]);
    }

    fprintf(stdout, "set/unset only");
    return (EXIT_FAILURE);
}

static int setEnvironment(const char *envVariable, long overwrite) {
    unsigned long size = strchr(envVariable, '=') - envVariable;
    char *key = malloc(size);
    strncpy(key, envVariable, size);

    if (getenv(key) && overwrite == 0) {
        fprintf(stderr, "key %s existed and overwrite is %ld\n", key, overwrite);
        free(key);
        key = NULL;
        return (EXIT_SUCCESS);
    }

    char *tmpEnv = malloc(strlen(envVariable));
    strcpy(tmpEnv, envVariable);
    if (putenv(tmpEnv) == -1) {
        fprintf(stderr, "failed to put env, %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "%s is set\n", getenv(key));

    free(key);
    key = NULL;

    return (EXIT_SUCCESS);
}

static void showEnv() {
    int i = 0;
    char **ep = environ;

    while (*ep != NULL) {
        fprintf(stdout, "%d %s\n", ++i, *ep);
        ep++;
    }
}

static int unsetEnvironment(const char *envName) {
    if (!getenv(envName)) {
        fprintf(stdout, "not exist\n");
        return (EXIT_SUCCESS);
    }

    showEnv();

    char **ep = environ;
    char *tmpEnvName = malloc(strlen(envName) + 1);
    strcpy(tmpEnvName, envName);
    tmpEnvName[strlen(envName)] = '=';

    while (*ep != NULL) {
        if (strncmp(*ep, tmpEnvName, strlen(envName) + 1) == 0) {
            char **eep = ep;
            while (*eep) {
                ++eep;
                *ep = *eep;
                ++ep;
            }

            showEnv();

            free(tmpEnvName);
            tmpEnvName = NULL;

            return (EXIT_SUCCESS);
        }
        ep++;
    }

    free(tmpEnvName);
    tmpEnvName = NULL;

    return (EXIT_SUCCESS);
}

