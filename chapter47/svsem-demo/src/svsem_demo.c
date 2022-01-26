#include "svsem_demo.h"
#include "semun.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int svsemDemo(int argc, char *argv[]) {
    int semid;
    char *endPtr = NULL;

    if (argc < 2 || argc > 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s init-value\n     or: %s semid operation\n", argv[0], argv[0]);
        return (EXIT_FAILURE);
    }

    if (argc == 2) {
        union semun arg;

        semid = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR);
        if (semid == -1) {
            fprintf(stderr, "failed to call semget(), %d\n", errno);
            return (EXIT_FAILURE);
        }

        arg.val = (int) strtol(argv[1], &endPtr, 10);
        endPtr = NULL;

        if (semctl(semid, 0, SETVAL, arg) == -1) {
            fprintf(stderr, "failed to call semctl(), %d\n", errno);
            return (EXIT_FAILURE);
        }

        fprintf(stdout, "Semaphore ID = %d\n", semid);
    } else {
        struct sembuf sop;

        semid = (int) strtol(argv[1], &endPtr, 10);
        endPtr = NULL;

        sop.sem_num = 0;
        sop.sem_op = (short) strtol(argv[2], &endPtr, 10);
        endPtr = NULL;

        sop.sem_flg = 0;

        time_t t = time(NULL);
        fprintf(stdout, "%ld: about to smop at %ld\n", (long) getpid(), (long) t);
        if (semop(semid, &sop, 1) == -1) {
            fprintf(stderr, "failed to call semop(), %d\n", errno);
            return (EXIT_FAILURE);
        }

        t = time(NULL);
        fprintf(stdout, "%ld: semop completed at %ld\n", (long) getpid(), (long) t);
    }

    return (EXIT_SUCCESS);
}