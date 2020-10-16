#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern char **environ;

int main() {
    char **ep;

    for (ep = environ; *ep != NULL; ep++) {
        puts(*ep);
    }

    return (EXIT_SUCCESS);
}