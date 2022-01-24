#ifndef THE_LINUX_PROGRAMMING_INTERFACE_SVMSG_FILE_H
#define THE_LINUX_PROGRAMMING_INTERFACE_SVMSG_FILE_H

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stddef.h>
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define SERVER_KEY 0x1aaaaaa1

struct requestMsg {
    long mtype;
    int clintId;
    char pathname[PATH_MAX];
};

#define REQ_MSG_SIZE (offsetof(struct requestMsg, pathname) - offsetof(struct requestMsg, clintId) + PATH_MAX)

#define RESP_MSG_SIZE 8192

struct responseMsg {
    long mtype;
    char data[RESP_MSG_SIZE];
};

#define RESP_MT_FAILURE 1
#define RESP_MT_DATA 2
#define RESP_MT_END 3

#endif //THE_LINUX_PROGRAMMING_INTERFACE_SVMSG_FILE_H
