#ifndef THE_LINUX_PROGRAMMING_INTERFACE_SVMSG_RM_H
#define THE_LINUX_PROGRAMMING_INTERFACE_SVMSG_RM_H

#include <sys/types.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int svmsgRm(int, char *[]);

#endif //THE_LINUX_PROGRAMMING_INTERFACE_SVMSG_RM_H
