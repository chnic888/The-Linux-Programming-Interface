#ifndef THE_LINUX_PROGRAMMING_INTERFACE_BINARY_SEMS_H
#define THE_LINUX_PROGRAMMING_INTERFACE_BINARY_SEMS_H

#include <stdbool.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

extern bool bsUseSemUndo;
extern bool bsRetryOnEintr;

int initSemAvailable(int semId, int semNum);

int initSemInUse(int semId, int semNum);

int reserveSem(int semId, int semNum);

int releaseSem(int semId, int semNum);

#endif //THE_LINUX_PROGRAMMING_INTERFACE_BINARY_SEMS_H
