#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("Hello World\n");
    write(STDIN_FILENO, "Ciao\n", 5);

    if (fork() == -1) {
        fprintf(stderr, "failed to fork()\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}