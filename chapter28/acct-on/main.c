#define _DEFAULT_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc > 2 || (argc > 1 && strcmp(argv[1], "--help") == 0)) {
        fprintf(stderr, "%s [file]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    if (acct(argv[1]) == -1) {
        fprintf(stderr, "failed to call acct(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Process accounting %s\n", (argv[1] == NULL) ? "disabled" : "enabled");
    exit(EXIT_SUCCESS);
}