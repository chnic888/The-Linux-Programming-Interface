#include "shadow_functions.h"
#include <shadow.h>
#include <stdio.h>
#include <stdlib.h>

static void getShadowByName(const char *);

static void printShadow();

int shadowFunctions(int argc, char *argv[]) {
    if (argc > 2) {
        fprintf(stderr, "%s loginname\n", argv[0]);
        return (EXIT_FAILURE);
    }

    if (argc == 2) {
        getShadowByName(argv[1]);
    } else {
        printShadow();
    }

    return (EXIT_SUCCESS);
}

static void getShadowByName(const char *name) {
    struct spwd *spwd = getspnam(name);
    if (spwd != NULL) {
        fprintf(stdout, "login name: %s\n", spwd->sp_namp);
        fprintf(stdout, "encrypted password: %s\n\n", spwd->sp_pwdp);
    }
}

static void printShadow() {
    struct spwd *spwd;

    while ((spwd = getspent()) != NULL) {
        fprintf(stdout, "login name: %s\n", spwd->sp_namp);
        fprintf(stdout, "encrypted password: %s\n\n", spwd->sp_pwdp);
    }

    endspent();
}
