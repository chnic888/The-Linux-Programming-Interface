#ifndef THE_LINUX_PROGRAMMING_INTERFACE_SVMSG_SEND_H
#define THE_LINUX_PROGRAMMING_INTERFACE_SVMSG_SEND_H

#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int svmsgSend(int, char *[]);

#endif //THE_LINUX_PROGRAMMING_INTERFACE_SVMSG_SEND_H
