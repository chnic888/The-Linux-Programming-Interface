#include "svsem_setall.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "semun.h"
#include <unistd.h>

int svsemSetall(int argc, char *argv[]) {
    struct semid_ds ds;
    union semun arg;
    int j, semid;
    char *endPtr = NULL;

    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s semid val...\n", argv[0]);
        return (EXIT_FAILURE);
    }

    semid = (int) strtol(argv[1], &endPtr, 10);
    endPtr = NULL;

    arg.buf = &ds;
    if (semctl(semid, 0, IPC_STAT, arg) == -1) {
        fprintf(stderr, "failed to call semctl() IPC_STAT, %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (ds.sem_nsems != argc - 2) {
        fprintf(stderr, "Set contains %ld semaphores, but %d values were supplied\n", (long) ds.sem_nsems, argc - 2);
        return (EXIT_FAILURE);
    }

    arg.array = calloc(ds.sem_nsems, sizeof(arg.array[0]));
    if (arg.array == NULL) {
        fprintf(stderr, "failed to call calloc\n");
        return (EXIT_FAILURE);
    }

    for (j = 2; j < argc; j++) {
        arg.array[j - 2] = strtol(argv[j], &endPtr, 10);
        endPtr = NULL;
    }

    if (semctl(semid, 0, SETALL, arg) == -1) {
        fprintf(stderr, "failed to call semctl() SETALL, %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Semaphore values changed (PID=%ld)\n", (long) getpid());

    return (EXIT_SUCCESS);
}