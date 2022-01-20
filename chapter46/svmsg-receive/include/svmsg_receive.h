#ifndef THE_LINUX_PROGRAMMING_INTERFACE_SVMSG_RECEIVE_H
#define THE_LINUX_PROGRAMMING_INTERFACE_SVMSG_RECEIVE_H

#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int svmsgReceive(int, char *[]);

#endif //THE_LINUX_PROGRAMMING_INTERFACE_SVMSG_RECEIVE_H
