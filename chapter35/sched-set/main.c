#include <stdio.h>
#include <sched.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    int j, pol;
    struct sched_param sp;
    char *endPtr = NULL;

    if (argc < 3 || strchr("rfobi", argv[1][0]) == NULL) {
        fprintf(stderr, "%s policy priority [pid...]\n"
                        "   policy is 'r' (RR), 'f' (FIFO), "
                        #ifdef SCHED_BATCH
                        "'b' (BATCH), "
                        #endif
                        #ifdef SCHED_IDLE
                        "'i' (IDLE), "
                        #endif
                        "or 'o' (OTHER)\n", argv[0]);
    }

    pol = (argv[1][0] == 'r') ? SCHED_RR : (argv[1][0] == 'f') ? SCHED_FIFO :
                                           #ifdef SCHED_BATCH
                                           (argv[1][0] == 'b') ? SCHED_BATCH :
                                           #endif
                                           #ifdef SCHED_IDLE
                                           (argv[1][0] == 'i') ? SCHED_IDLE :
                                           #endif
                                           SCHED_OTHER;

    sp.sched_priority = (int) strtol(argv[2], &endPtr, 10);
    endPtr = NULL;

    for (j = 3; j < argc; j++) {
        if (sched_setscheduler((int) strtol(argv[j], &endPtr, 10), pol, &sp) == -1) {
            fprintf(stderr, "failed to call sched_setscheduler(), %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}