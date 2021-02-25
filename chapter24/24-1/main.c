#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (fork() == 0) {
        fprintf(stdout, "%d - %d\n", 1, getpid());
    }

    if (fork() == 0) {
        fprintf(stdout, "%d - %d\n", 2, getpid());
    }

    if (fork() == 0) {
        fprintf(stdout, "%d - %d\n", 3, getpid());
    }

    sleep(3);
    return (EXIT_SUCCESS);
}