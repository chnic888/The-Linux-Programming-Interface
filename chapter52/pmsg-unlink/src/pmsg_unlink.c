#include "pmsg_unlink.h"
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int pmsgUnlink(int argc, char *argv[]) {
    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s mq-name\n", argv[0]);
        return (EXIT_FAILURE);
    }

    if (mq_unlink(argv[1]) == -1) {
        fprintf(stderr, "failed to call mq_unlink(), %d\n", errno);
    }

    return (EXIT_SUCCESS);
}