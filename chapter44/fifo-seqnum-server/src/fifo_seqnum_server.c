#include "fifo_seqnum_server.h"
#include "fifo_seqnum.h"
#include <signal.h>
#include <errno.h>


int fifoSeqnumServer(int argc, char *argv[]) {
    int serverFd, dummyFd, clientFd;
    char clientFifo[CLIENT_FIFO_NAME_LEN];
    struct request req;
    struct response resp;
    int seqNum = 0;

    umask(0);
    if (mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST) {
        fprintf(stderr, "mkfifo %s\n", SERVER_FIFO);
        return EXIT_FAILURE;
    }

    serverFd = open(SERVER_FIFO, O_RDONLY);
    if (serverFd == -1) {
        fprintf(stderr, "open %s\n", SERVER_FIFO);
        return EXIT_FAILURE;
    }

    dummyFd = open(SERVER_FIFO, O_WRONLY);
    if (dummyFd == -1) {
        fprintf(stderr, "open %s\n", SERVER_FIFO);
        return EXIT_FAILURE;
    }

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        fprintf(stderr, "signal\n");
        return EXIT_FAILURE;
    }

    for (;;) {
        if (read(serverFd, &req, sizeof(struct request)) != sizeof(struct request)) {
            fprintf(stderr, "Error reading request; discarding\n");
            continue;
        }

        snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLE, (long) req.pid);
        clientFd = open(clientFifo, O_WRONLY);
        if (clientFd == -1) {
            fprintf(stderr, "open %s\n", clientFifo);
            continue;
        }

        resp.seqNum = seqNum;
        if (write(clientFd, &resp, sizeof(struct response)) != sizeof(struct response)) {
            fprintf(stderr, "Error writing to FIFO %s\n", clientFifo);
        }

        if (close(clientFd) == -1) {
            fprintf(stderr, "close\n");
        }

        seqNum += req.seqlen;
    }
}