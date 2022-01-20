#define GNU_SOURCE

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
    
    return (EXIT_SUCCESS);
}