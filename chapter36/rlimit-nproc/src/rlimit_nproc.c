#include "rlimit_nproc.h"
#include "print_rlimit.h"
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int rlimitNproc(int argc, char *argv[]) {
    struct rlimit rl;
    int j;
    pid_t childPid;
    char *endPtr = NULL;

    if (argc < 2 || argc > 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s soft-limit [hard-limit]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printRlimit("Initial maximum process limits: ", RLIMIT_NPROC);

    rl.rlim_cur = (argv[1][0] == 'i') ? RLIM_INFINITY : strtol(argv[1], &endPtr, 10);
    endPtr = NULL;

    rl.rlim_max = (argc == 2) ? rl.rlim_cur : (argv[1][0] == 'i') ? RLIM_INFINITY : strtol(argv[2], &endPtr, 10);
    endPtr = NULL;

    if (setrlimit(RLIMIT_NPROC, &rl) == -1) {
        fprintf(stderr, "failed to call setrlimit(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    printRlimit("New maximum process limits:     ", RLIMIT_NPROC);

    for (j = 1; j < 10; j++) {
        switch (childPid = fork()) {
            case -1:
                fprintf(stderr, "failed to call fork(), %d\n", errno);
                exit(EXIT_FAILURE);
            case 0:
                _exit(EXIT_SUCCESS);
            default:
                fprintf(stdout, "Child %d (PID=%ld) started\n", j, (long) childPid);
                break;
        }
    }

	return (EXIT_SUCCESS);
}