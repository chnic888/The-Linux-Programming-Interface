#include "svsem_good_init.h"
#include "semun.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int svsemGoodInit(int argc, char *argv[]) {
    char *endPtr = NULL;
    int semid, key;

    key = (int) strtol(argv[1], &endPtr, 10);
    semid = semget(key, 1, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    if (semid != -1) {
        union semun arg;
        struct sembuf sop;

        arg.val = 0;
        if (semctl(semid, 0, SETVAL, arg) == -1) {
            fprintf(stderr, "failed to call semctl(), %d\n", errno);
            return (EXIT_FAILURE);
        }

        sop.sem_num = 0;
        sop.sem_op = 0;
        sop.sem_flg = 0;
        if (semop(semid, &sop, 1) == -1) {
            fprintf(stderr, "failed to call semop(), %d\n", errno);
            return (EXIT_FAILURE);
        }
    } else {
        const int MAX_TRIES = 10;
        int j;
        union semun arg;
        struct semid_ds ds;

        if (errno != EEXIST) {
            fprintf(stderr, "semaphore not exist\n");
            return (EXIT_FAILURE);
        }

        semid = semget(key, 1, S_IRUSR | S_IWUSR);
        if (semid == -1) {
            fprintf(stderr, "failed to call semget(), %d\n", errno);
            return (EXIT_FAILURE);
        }

        arg.buf = &ds;
        for (j = 0; j < MAX_TRIES; j++) {
            if (semctl(semid, 0, IPC_STAT, arg) == -1) {
                fprintf(stderr, "failed to call semctl(), %d\n", errno);
            }

            if (ds.sem_otime != 0) {
                break;
            }

            sleep(1);
        }

        if (ds.sem_otime == 0) {
            fprintf(stderr, "Existing semaphore not initialized\n");
            return (EXIT_FAILURE);
        }
    }

    return (EXIT_SUCCESS);
}