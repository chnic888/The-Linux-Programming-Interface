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
    return (EXIT_SUCCESS);
}