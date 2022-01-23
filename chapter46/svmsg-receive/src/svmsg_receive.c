#define _GNU_SOURCE

#include <errno.h>
#include "svmsg_receive.h"

#define MAX_MTEXT 1024

struct mbuf {
    long mtype;
    char mtext[MAX_MTEXT];
};

static void usageError(const char *progName, const char *msg) {
    if (msg != NULL) {
        fprintf(stderr, "%s", msg);
    }

    fprintf(stderr, "Usage: %s [options] msqid [max-byte]\n", progName);
    fprintf(stderr, "Permitted options are:\n");
    fprintf(stderr, "   -e      Use MSG_NOERROR flag\n");
    fprintf(stderr, "   -t type Select message of given type\n");
    fprintf(stderr, "   -n      Use IPC_NOWAIT flag\n");

#ifdef MSG_EXCEPT
    fprintf(stderr, "   -x      Use MSG_EXCEPT flag\n");
#endif
    exit(EXIT_FAILURE);
}

int svmsgReceive(int argc, char *argv[]) {
    int msqid, flags, type;
    ssize_t msglen;
    size_t maxBytes;
    struct mbuf msg;
    int opt;
    char *endPtr = NULL;

    flags = 0;
    type = 0;

    while ((opt = getopt(argc, argv, "ent:x")) != -1) {
        switch (opt) {
            case 'e':
                flags |= MSG_NOERROR;
                break;

            case 'n':
                flags |= IPC_NOWAIT;
                break;

            case 't':
                type = strtol(optarg, &endPtr, 10);
                endPtr = NULL;
                break;
#ifdef MSG_EXCEPT
                case 'x':
                flags |= MSG_EXCEPT;
                break;
#endif
            default:
                usageError(argv[0], NULL);
        }
    }

    if (argc < optind + 1 || argc > optind + 2) {
        usageError(argv[0], "Wrong number of arguments\n");
    }

    msqid = strtol(argv[optind], &endPtr, 10);
    endPtr = NULL;
    maxBytes = (argc > optind + 1) ? strtol(argv[optind + 1], &endPtr, 10) : MAX_MTEXT;

    msglen = msgrcv(msqid, &msg, maxBytes, type, flags);
    if (msglen == -1) {
        fprintf(stderr, "failed to call msgrev(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Received: ty[e-%ld; length=%ld", msg.mtype, (long) msglen);
    if (msglen > 0) {
        fprintf(stdout, "; body=%s", msg.mtext);
    }
    printf("\n");

    return (EXIT_SUCCESS);
}