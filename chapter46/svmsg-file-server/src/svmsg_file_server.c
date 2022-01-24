#include "svmsg_file_server.h"
#include "svmsg_file.h"

static void grimReaper(int sig) {
    int savedErrno;

    savedErrno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        continue;
    }

    errno = savedErrno;
}

static void serveRequest(const struct requestMsg *req) {
    int fd;
    ssize_t numRead;
    struct responseMsg resp;

    fd = open(req->pathname, O_RDONLY);
    if (fd == -1) {
        resp.mtype = RESP_MT_FAILURE;
        snprintf(resp.data, sizeof(resp.data), "%s", "Couldn't open");
        msgsnd(req->clintId, &resp, strlen(resp.data) + 1, 0);
        exit(EXIT_FAILURE);
    }

    resp.mtype = RESP_MT_DATA;
    while ((numRead = read(fd, resp.data, RESP_MSG_SIZE)) > 0) {
        if (msgsnd(req->clintId, &resp, numRead, 0) == -1) {
            break;
        }
    }

    resp.mtype = RESP_MT_END;
    msgsnd(req->clintId, &resp, 0, 0);
}

int svmsgFileServer(int argc, char *argv[]) {
    struct requestMsg req;
    pid_t pid;
    ssize_t msgLen;
    int serverId;
    struct sigaction sa;

    serverId = msgget(SERVER_KEY, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IWGRP);
    if (serverId == -1) {
        fprintf(stderr, "failed to call msgget(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = grimReaper;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        fprintf(stderr, "failed to call sigaction(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    for (;;) {
        msgLen = msgrcv(serverId, &req, REQ_MSG_SIZE, 0, 0);
        if (msgLen == -1) {
            if (errno == EINTR) {
                continue;
            }

            fprintf(stderr, "failed to call msgrcv, %d\n", errno);
            break;
        }

        pid = fork();
        if (pid == -1) {
            fprintf(stderr, "failed to call fork(), %d\n", errno);
            break;
        }

        if (pid == 0) {
            serveRequest(&req);
            _exit(EXIT_SUCCESS);
        }
    }

    if (msgctl(serverId, IPC_RMID, NULL) == -1) {
        fprintf(stderr, "failed to call msgctl, %d\n", errno);
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}