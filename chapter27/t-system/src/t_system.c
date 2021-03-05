#include "t_system.h"
#include "print_wait_status.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define MAX_CMD_LEN 200

int tSystem(int argc, char *argv[]) {
    char str[MAX_CMD_LEN];
    int status;

    //read and execute shell command
    for (;;) {
        fprintf(stdout, "Command: ");
        fflush(stdout);

        if (fgets(str, MAX_CMD_LEN, stdin) == NULL)
            break;

        status = system(str);
        fprintf(stdout, "system() returned: status=0x%04x (%d,%d)\n", (unsigned int) status, status >> 8,
                status & 0xff);

        if (status == -1) {
            fprintf(stderr, "failed to call system(), %d\n", errno);
            exit(EXIT_FAILURE);
        } else {
            if (WIFEXITED(status) && WEXITSTATUS(status) == 127) {
                fprintf(stdout, "(Probably) could not invoke shell\n");
            } else {
                //shell execute successfully
                printWaitStatus(NULL, status);
            }
        }
    }

    exit(EXIT_SUCCESS);
}