#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    fprintf(stdout, "Initial value of USER: %s\n", getenv("USER"));

    if (putenv("USER=britta") != 0) {
        fprintf(stderr, "failed to call putenv(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    execl("/usr/bin/printenv", "printenv", "USER", "SHELL", (char *) NULL);

    fprintf(stderr, "failed to call execl(), %d\n", errno);
    exit(EXIT_FAILURE);
}
