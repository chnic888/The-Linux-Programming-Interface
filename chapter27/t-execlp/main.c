#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stdout, "%s pathname\n", argv[0]);
        return (EXIT_FAILURE);
    }

    execlp(argv[1], argv[1], "hello world", (char *) NULL);

    fprintf(stderr, "failed to call execlp(), %d\n", errno);
    exit(EXIT_FAILURE);
}