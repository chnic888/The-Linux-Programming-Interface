#include "svmsg_create.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>


static void usageError(const char *progName, const char *msg) {
    if (msg != NULL) {
        fprintf(stderr, "%s", msg);
    }

    fprintf(stderr, "Usage: %s [-cx] {-f pathname | -k key | -p} [octal-perms]\n", progName);
    fprintf(stderr, "   -c              Use IPC_CREAT flag\n");
    fprintf(stderr, "   -x              Use IPC_CREAT flag\n");
    fprintf(stderr, "   -f pathname     Generate key using ftok()\n");
    fprintf(stderr, "   -k key          User 'key' as key\n");
    fprintf(stderr, "   -p              User IPC_PRIVATE key\n");
    exit(EXIT_FAILURE);
}

int svmsgCreate(int argc, char *argv[]) {
    int numKeyFlags;
    int flags, msqid, opt;
    unsigned int perms;
    long lkey;
    key_t key;
    char *endPtr = NULL;

    numKeyFlags = 0;
    flags = 0;

    while ((opt = getopt(argc, argv, "cf:k:px")) != -1) {
        switch (opt) {
            case 'c':
                flags |= IPC_CREAT;
                break;

            case 'f':
                key = ftok(optarg, 1);
                if (key == -1) {
                    fprintf(stderr, "failed to call ftok(), %d\n", errno);
                    exit(EXIT_FAILURE);
                }
                numKeyFlags++;
                break;

            case 'k':
                if (sscanf(optarg, "%li", &lkey) != 1) {
                    fprintf(stderr, "-k option requires a numeric argument\n");
                    exit(EXIT_FAILURE);
                }
                key = lkey;
                numKeyFlags++;
                break;

            case 'p':
                key = IPC_PRIVATE;
                numKeyFlags++;
                break;

            case 'x':
                flags |= IPC_EXCL;
                break;

            default:
                usageError(argv[0], "Bad option\n");
        }
    }

    if (numKeyFlags != 1) {
        fprintf(stderr, "Exactly one of options -f, -k, or -p must be supplied\n");
        exit(EXIT_FAILURE);
    }

    perms = (optind == argc) ? (S_IRUSR | S_IWUSR) : strtol(argv[optind], &endPtr, 10);
    msqid = msgget(key, flags | perms);
    if (msqid == -1) {
        fprintf(stderr, "failed to call msgget(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "%d\n", msqid);
    exit(EXIT_SUCCESS);
}