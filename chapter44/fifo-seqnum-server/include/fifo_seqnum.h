#ifndef THE_LINUX_PROGRAMMING_INTERFACE_FIFO_SEQNUM_H
#define THE_LINUX_PROGRAMMING_INTERFACE_FIFO_SEQNUM_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_FIFO "/tmp/seqnum_sv"
#define CLIENT_FIFO_TEMPLE "/tmp/seqnum_cl.%ld"
#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLE) + 20)

struct request {
    pid_t pid;
    int seqlen;
};

struct response {
    int seqNum;
};

#endif //THE_LINUX_PROGRAMMING_INTERFACE_FIFO_SEQNUM_H
