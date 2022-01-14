#include "fifo_seqnum_client.h"
#include "fifo_seqnum.h"
#include <string.h>
#include <errno.h>

static char clientFifo[CLIENT_FIFO_NAME_LEN];

static void removeFifo(void) {
    unlink(clientFifo);
}

int fifoSeqnumClient(int argc, char *argv[]) {
    int serverFd, clientFd;
    struct request req;
    struct response resp;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s [seq-len...]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    umask(0);
    snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLE, (long) getpid());

    if (mkfifo(clientFifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST) {
        fprintf(stderr, "mkfifo %s\n", clientFifo);
        exit(EXIT_FAILURE);
    }

    if (atexit(removeFifo) != 0) {
        exit(EXIT_FAILURE);
    }

    req.pid = getpid();
    char *endPtr;
    req.seqlen = (argc > 1) ? (int) strtol(argv[1], &endPtr, 0) : 0;
    endPtr = NULL;

    serverFd = open(SERVER_FIFO, O_WRONLY);
    if (serverFd == -1) {
        fprintf(stderr, "open %s\n", SERVER_FIFO);
        exit(EXIT_FAILURE);
    }

    if (write(serverFd, &req, sizeof(struct request)) != sizeof(struct request)) {
        fprintf(stderr, "Can't write to server\n");
        exit(EXIT_FAILURE);
    }

    clientFd = open(clientFifo, O_RDONLY);
    if (clientFd == -1) {
        fprintf(stderr, "open %s\n", SERVER_FIFO);
        exit(EXIT_FAILURE);
    }

    if (read(clientFd, &resp, sizeof(struct response)) != sizeof(struct response)) {
        fprintf(stderr, "Can't read response from server\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "%d\n", resp.seqNum);
    exit(EXIT_SUCCESS);
}

