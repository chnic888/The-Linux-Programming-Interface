#include <stdio.h>
#include <stdlib.h>

extern char **environ;

int main(int argc, char *argv[]) {
    int j;
    char **ep;

    for (j = 0; j < argc; j++) {
        fprintf(stdout, "argv[%d] = %s\n", j, argv[j]);
    }

    for (ep = environ; *ep != NULL; ep++) {
        fprintf(stdout, "environ: %s\n", *ep);
    }

    return (EXIT_SUCCESS);
}