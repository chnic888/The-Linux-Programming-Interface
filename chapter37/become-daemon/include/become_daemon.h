#ifndef THE_LINUX_PROGRAMMING_INTERFACE_BECOME_DAEMON_H

#define THE_LINUX_PROGRAMMING_INTERFACE_BECOME_DAEMON_H

#define BD_NO_CHDIR 01
#define BD_NO_CLOSE_FILES 02
#define BD_NO_REOPEN_STD_FDS 04

#define BD_NO_UMASK 010

#define BD_MAX_CLOSE 8192

int becomeDaemon(int);

#endif //THE_LINUX_PROGRAMMING_INTERFACE_BECOME_DAEMON_H
