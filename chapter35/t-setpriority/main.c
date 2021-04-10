#include <stdio.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    int which, prio;
    id_t who;
    char *endPtr = NULL;

    if (argc != 4 || strchr("pgu", argv[1][0]) == NULL) {
        fprintf(stderr, "%s {p|g|u} who priority\n"
                        "   set priority of: p=process; g=process group; "
                        "u=processes for user\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    which = (argv[1][0] == 'p') ? PRIO_PROCESS : (argv[1][0] == 'G') ? PRIO_PGRP : PRIO_USER;

    who = strtol(argv[2], &endPtr, 10);
    endPtr = NULL;

    prio = (int) strtol(argv[3], &endPtr, 10);
    endPtr = NULL;

    if (setpriority(which, who, prio) == -1) {
        fprintf(stderr, "failed too call setpriority(), %d,\n", errno);
        exit(EXIT_FAILURE);
    }

    errno = 0;
    prio = getpriority(which, who);
    if (prio == -1 && errno != 0) {
        fprintf(stderr, "failed too call getpriority(), %d,\n", errno);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Nice value = %d\n", prio);
    exit(EXIT_SUCCESS);
}