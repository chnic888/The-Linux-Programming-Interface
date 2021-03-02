#include "make_zombie.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <libgen.h>
#include <unistd.h>

#define CMD_SIZE 200

int makeZombie(int argc, char *argv[]) {
    char cmd[CMD_SIZE];
    pid_t childPid;

    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    fprintf(stdout, "Parent PID=%ld\n", (long) getpid());

    switch (childPid = fork()) {
        case -1:
            break;
        case 0:
            fprintf(stdout, "Child (PID=%ld) exiting\n", (long) getpid());
            _exit(EXIT_SUCCESS);
        default:
            sleep(3);
            snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(argv[0]));
            cmd[CMD_SIZE - 1] = '\0';
            system(cmd);

            if (kill(childPid, SIGKILL) == -1) {
                fprintf(stderr, "failed to call kill(), %d\n", errno);
                exit(EXIT_FAILURE);
            }

            sleep(3);
            fprintf(stdout, "After sending SIGKILL to zombie (PID=%ld):\n", (long) childPid);
            system(cmd);

            exit(EXIT_SUCCESS);
    }

    return (EXIT_SUCCESS);
}