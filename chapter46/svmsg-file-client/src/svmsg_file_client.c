#include "svmsg_file_client.h"
#include "svmsg_file.h"

static int clientId;

static void removeQueue(void) {
    if (msgctl(clientId, IPC_RMID, NULL) == -1) {
        fprintf(stderr, "failed to call msgctl(), %d\n", errno);
    }
}

int svmsgFileClient(int argc, char *argv[]) {
    struct requestMsg req;
    struct responseMsg resp;
    int serverId, numMsgs;
    ssize_t msgLen, totBytes;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s pathname\n", argv[0]);
        return (EXIT_FAILURE);
    }

    if (strlen(argv[1]) > sizeof(req.pathname) - 1) {
        fprintf(stderr, "pathname too long (max: %ld bytes)\n", (long) sizeof(req.pathname) - 1);
        return (EXIT_FAILURE);
    }

    serverId = msgget(SERVER_KEY, S_IWUSR);
    if (serverId == -1) {
        fprintf(stderr, "msgget - server message queue\n");
        return (EXIT_FAILURE);
    }

    clientId = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | S_IWGRP);
    if (clientId == -1) {
        fprintf(stderr, "msgget - server message queue\n");
        return (EXIT_FAILURE);
    }

    if (atexit(removeQueue) != 0) {
        fprintf(stderr, "failed to call atexit(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    req.mtype = 1;
    req.clintId = clientId;
    strncpy(req.pathname, argv[1], sizeof(req.pathname) - 1);
    req.pathname[sizeof(req.pathname) - 1] = '\0';

    if (msgsnd(serverId, &req, REQ_MSG_SIZE, 0) == -1) {
        fprintf(stderr, "failed to call msgsnd, %d\n", errno);
        return (EXIT_FAILURE);
    }

    msgLen = msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
    if (msgLen == -1) {
        fprintf(stderr, "failed to call msgrcv(), %d", errno);
        return (EXIT_FAILURE);
    }

    if (resp.mtype == RESP_MT_FAILURE) {
        fprintf(stderr, "%s\n", resp.data);
        return (EXIT_FAILURE);
    }

    totBytes = msgLen;
    for (numMsgs = 1; resp.mtype == RESP_MT_DATA; numMsgs++) {
        msgLen = msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
        if (msgLen == -1) {
            fprintf(stderr, "failed to call msgrcv(), %d\n", errno);
            return (EXIT_FAILURE);
        }

        totBytes += msgLen;
    }

    fprintf(stdout, "Received %ld bytes (%d messages)\n", (long) totBytes, numMsgs);
    return (EXIT_SUCCESS);
}