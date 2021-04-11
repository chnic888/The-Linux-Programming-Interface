#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    int j, pol;
    struct sched_param sp;
    char *endPtr;

    for (j = 1; j < argc; j++) {
        int pid = (int) strtol(argv[j], &endPtr, 10);
        endPtr = NULL;

        pol = sched_getscheduler(pid);
        if (pol == -1) {
            fprintf(stderr, "failed to call sched_getscheduler(), %d\n", errno);
            exit(EXIT_FAILURE);
        }

        if (sched_getparam(pid, &sp) == -1) {
            fprintf(stderr, "failed to call sched_getparam(), %d\n", errno);
            exit(EXIT_FAILURE);
        }

        fprintf(stdout, "%s: %-5s %2d\n", argv[j],
                (pol == SCHED_OTHER) ? "OTHER" :
                (pol == SCHED_RR) ? "RR" :
                (pol == SCHED_FIFO) ? "FIFO" :
                #ifdef SCHED_BATCH
                (pol == SCHED_BATCH) ? "BATCH" :
                #endif
                #ifdef SCHED_IDLE
                (pol == SCHED_IDLE) ? "IDLE" :
                #endif
                "???", sp.sched_priority);
    }

    return (EXIT_SUCCESS);
}