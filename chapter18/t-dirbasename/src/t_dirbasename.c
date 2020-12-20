#include "t_dirbasename.h"
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <errno.h>

int tDirBaseName(int argc, char *argv[]) {
    char *t1, *t2;
    int j;

    for (j = 1; j < argc; j++) {
        t1 = strdup(argv[j]);
        if (t1 == NULL) {
            fprintf(stderr, "failed to call strdup(), %d\n", errno);
            return (EXIT_FAILURE);
        }

        t2 = strdup(argv[j]);
        if (t2 == NULL) {
            fprintf(stderr, "failed to call strdup(), %d\n", errno);
            return (EXIT_FAILURE);
        }

        fprintf(stdout, "%s ==> %s + %s\n", argv[j], dirname(t1), basename(t2));

        free(t1);
        free(t2);
    }

    return (EXIT_SUCCESS);
}