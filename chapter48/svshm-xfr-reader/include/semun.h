#ifndef THE_LINUX_PROGRAMMING_INTERFACE_SEMUN_H
#define THE_LINUX_PROGRAMMING_INTERFACE_SEMUN_H

#include <sys/types.h>
#include <sys/sem.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
#if defined(__linux__)
    struct seminfo *__buf;
#endif
};

#endif //THE_LINUX_PROGRAMMING_INTERFACE_SEMUN_H
