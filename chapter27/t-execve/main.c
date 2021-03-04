#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    char *argVec[10];
    char *envVec[] = {"GREET=salut", "BYE=adieu", NULL};

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s pathname\n", argv[0]);
        return (EXIT_FAILURE);
    }

    argVec[0] = strrchr(argv[1], '/') + 1;
    argVec[1] = "hello world";
    argVec[2] = "goodbye";
    argVec[3] = NULL;

    execve(argv[1], argVec, envVec);
    fprintf(stderr, "failed to call execve(), %d\n", errno);

    exit(EXIT_FAILURE);
}