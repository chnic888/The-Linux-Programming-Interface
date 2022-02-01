#include "svsem_rm.h"
#include <sys/types.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int svsemRm(int argc, char *argv[]) {
    int semid;
    char *endPtr = NULL;

    if (argc != 2) {
        fprintf(stderr, "%s semid\n", argv[0]);
        return (EXIT_FAILURE);
    }

    semid = (int) strtol(argv[1], &endPtr, 10);
    endPtr = NULL;

    if (semctl(semid, 0, IPC_RMID) == -1) {
        fprintf(stderr, "failed to delete %d semaphore, %d\n", semid, errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "delete semaphore %d successfully\n", semid);
    return (EXIT_SUCCESS);
}