#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    char *envVec[] = {"GREET=salut", "BYE=adieu", NULL};
    char *fileName;

    if (argc != 2 || strcmp(argv[1], "/") == 0) {
        fprintf(stderr, "%s pathname\n", argv[0]);
        return (EXIT_FAILURE);
    }

    fileName = strrchr(argv[1], '/');
    if (fileName != NULL)
        fileName++;
    else
        fileName = argv[1];

    execle(argv[1], fileName, "hello world", (char *) NULL, envVec);

    fprintf(stderr, "failed to call execle(), %d\n", errno);
    exit(EXIT_FAILURE);
}