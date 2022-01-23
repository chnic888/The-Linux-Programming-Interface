#define _GNU_SOURCE

#include "svmsg_ls.h"
#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int svmsgLs(int argc, char *argv[]) {
    int maxind, ind, msqid;
    struct msqid_ds ds;
    struct msginfo msginfo;

    maxind = msgctl(0, MSG_INFO, (struct msqid_ds *) &msginfo);
    if (maxind == -1) {
        fprintf(stderr, "msgctl-MSG_INFO\n");
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "maxind: %d\n\n", maxind);
    fprintf(stdout, "index       id      key         message\n");

    for (ind = 0; ind <= maxind; ind++) {
        msqid = msgctl(ind, MSG_STAT, &ds);
        if (msqid == -1) {
            if (errno != EINVAL && errno != EACCES) {
                fprintf(stderr, "msgctl-MSG_STAT");
            }
            continue;
        }

        fprintf(stdout, "%4d %8d 0x%08lx %7ld\n", ind, msqid, (unsigned long) ds.msg_perm.__key, (long) ds.msg_qnum);
    }

    return (EXIT_SUCCESS);
}