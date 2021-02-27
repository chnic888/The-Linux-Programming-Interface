#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

static int idata = 111;

int main(int argc, char *argv[]) {
    int istack = 222;
    pid_t childPid;

    switch (childPid = fork()) {
        case -1:
            fprintf(stderr, "failed to create process, %d\n", errno);
            exit(EXIT_FAILURE);
        case 0:
            idata *= 3;
            istack *= 3;
            break;
        default:
            sleep(3);
            break;
    }

    fprintf(stdout, "PID=%ld %s idata=%d istack=%d\n", (long) getpid(), (childPid == 0) ? "(child) " : "(parent)", idata, istack);
    exit(EXIT_SUCCESS);
}