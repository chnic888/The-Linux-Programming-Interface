#include "svmsg_rm.h"

int svmsgRm(int argc, char *argv[]) {
    int j;
    char *endPtr;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s [msqid...]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    for (j = 1; j < argc; j++) {
        if (msgctl((int) strtol(argv[j], &endPtr, 10), IPC_RMID, NULL) == -1) {
            fprintf(stderr, "msgctl %s, %d\n", argv[j], errno);
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}