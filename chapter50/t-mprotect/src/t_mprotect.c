#define _DEFAULT_SOURCE

#include "t_mprotect.h"
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define LEN (1024 * 1024)

#define SHELL_FMT "cat /proc/%ld/maps | grep zero"
#define CMD_SIZE (sizeof(SHELL_FMT) + 20)

int tMprotect(int argc, char *argv[]) {
    char cmd[CMD_SIZE];
    char *addr;

    addr = mmap(NULL, LEN, PROT_NONE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED) {
        fprintf(stderr, "failed to call mmap(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Before mprorect()\n");
    snprintf(cmd, CMD_SIZE, SHELL_FMT, (long) getpid());
    system(cmd);

    if (mprotect(addr, LEN, PROT_READ | PROT_WRITE) == -1) {
        fprintf(stderr, "failed to call mprotect(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "After mprotect()\n");
    system(cmd);

    return (EXIT_SUCCESS);
}