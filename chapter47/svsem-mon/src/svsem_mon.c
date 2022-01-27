#include "svsem_mon.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "semun.h"
#include <string.h>
#include <errno.h>

int svsemMon(int argc, char *argv[]) {
    struct semid_ds ds;
    union semun arg, dummy;
    int semid, j;
    char *endPtr = NULL;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s semid\n", argv[0]);
        return (EXIT_FAILURE);
    }

    semid = (int) strtol(argv[1], &endPtr, 10);
    endPtr = NULL;

    arg.buf = &ds;
    if (semctl(semid, 0, IPC_STAT, arg) == -1) {
        fprintf(stderr, "failed to call semctl(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Semaphore changed: %s", ctime(&ds.sem_ctime));
    fprintf(stdout, "Last semop()       %s", ctime(&ds.sem_otime));

    arg.array = calloc(ds.sem_nsems, sizeof(arg.array[0]));
    if (arg.array == NULL) {
        fprintf(stderr, "failed to calloc(), %d\n", errno);
        return (EXIT_FAILURE);
    }
    if (semctl(semid, 0, GETALL, arg) == -1) {
        fprintf(stderr, "failed to semctl() GETALL, %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Sem #  Value   SEMPID   SEMNCNT   SEMZNT\n");

    for (j = 0; j < ds.sem_nsems; j++) {
        fprintf(stdout, "%3d    %5d    %5d    %5d    %5d\n", j, arg.array[j], semctl(semid, j, GETPID, dummy),
                semctl(semid, j, GETNCNT, dummy), semctl(semid, j, GETZCNT, dummy));
    }

    return (EXIT_SUCCESS);
}