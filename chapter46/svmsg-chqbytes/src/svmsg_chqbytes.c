#include "svmsg_chqbytes.h"
#include <stdlib.h>
#include <sys/msg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int svmsgChqbytes(int argc, char *argv[]) {
    struct msqid_ds ds;
    int msqid;
    char *endPtr = NULL;

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s msqid max-byte\n", argv[0]);
        return (EXIT_FAILURE);
    }

    msqid = (int) strtol(argv[1], &endPtr, 10);
    endPtr = NULL;

    if (msgctl(msqid, IPC_STAT, &ds) == -1) {
        fprintf(stderr, "failed to call msgctl(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    ds.msg_qbytes = strtol(argv[2], &endPtr, 10);
    endPtr = NULL;

    if (msgctl(msqid, IPC_SET, &ds) == -1) {
        fprintf(stderr, "failed to call msgctl(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    return (EXIT_FAILURE);
}
