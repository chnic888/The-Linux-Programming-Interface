#include "svmsg_send.h"

#define MAX_MTEXT 1024

struct mbuf {
    long mtype;
    char mtext[MAX_MTEXT];
};

static void usageError(const char *progName, const char *msg) {
    if (msg != NULL) {
        fprintf(stderr, "%s", msg);
    }

    fprintf(stderr, "Usage: %s [-n] msqid msg-type [msg-text]\n", progName);
    fprintf(stderr, "   -n      Use IPC_NOWAIT flag\n");
    exit(EXIT_FAILURE);
}

int svmsgSend(int argc, char *argv[]) {
    int msqid, flags, msgLen;
    struct mbuf msg;
    int opt;
    char *endPtr = NULL;

    flags = 0;
    while ((opt = getopt(argc, argv, "n")) != -1) {
        if (opt == 'n')
            flags |= IPC_NOWAIT;
        else
            usageError(argv[0], NULL);
    }

    if (argc < optind + 2 || argc > optind + 3) {
        usageError(argv[0], "Wrong number of arguments\n");
    }

    msqid = strtol(argv[optind], &endPtr, 10);
    endPtr = NULL;

    msg.mtype = strtol(argv[optind + 1], &endPtr, 10);
    endPtr = NULL;

    if (argc > optind + 2) {
        msgLen = strlen(argv[optind + 2]) + 1;
        if (msgLen > MAX_MTEXT) {
            fprintf(stderr, "msg-text too long (max: %d characters)\n", MAX_MTEXT);
            exit(EXIT_FAILURE);
        }

        memcpy(msg.mtext, argv[optind + 2], msgLen);
    } else {
        msgLen = 0;
    }

    if (msgsnd(msqid, &msg, msgLen, flags) == -1) {
        fprintf(stderr, "failed to call msgsnd(), %d\n", errno);
        exit(EXIT_FAILURE);
    }
    
    return (EXIT_SUCCESS);
}