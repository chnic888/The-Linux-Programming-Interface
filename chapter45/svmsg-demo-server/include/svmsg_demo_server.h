#ifndef THE_LINUX_PROGRAMMING_INTERFACE_CHAPTER45_SVMSG_DEMO_SERVER_SRC_SVMSG_DEMO_SERVER_H_
#define THE_LINUX_PROGRAMMING_INTERFACE_CHAPTER45_SVMSG_DEMO_SERVER_SRC_SVMSG_DEMO_SERVER_H_

#define KEY_FILE "/tmp/somefile"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int svmsgDemoServer(int, char **);

#endif //THE_LINUX_PROGRAMMING_INTERFACE_CHAPTER45_SVMSG_DEMO_SERVER_SRC_SVMSG_DEMO_SERVER_H_
