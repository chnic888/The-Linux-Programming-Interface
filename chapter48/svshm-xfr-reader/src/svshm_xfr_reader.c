#include "svshm_xfr_reader.h"
#include "svshm_xfr.h"

int svshmXfrReader(int argc, char *argv[]) {
    int semid, shmid, xfrs, bytes;
    struct shmseg *shmp;

    semid = semget(SEM_KEY, 0, 0);
    if (semid == -1) {
        fprintf(stderr, "failed to call semget(), %d\n", errno);
        return (EXIT_FAILURE);
    }
    fprintf(stdout, "semid = %d\n", semid);

    shmid = shmget(SHM_KEY, 0, 0);
    if (shmid == -1) {
        fprintf(stderr, "failed to call shmget(), %d\n", errno);
        return (EXIT_FAILURE);
    }
    fprintf(stdout, "shmid = %d\n", shmid);

    shmp = shmat(shmid, NULL, SHM_RDONLY);
    if (shmp == (void *) -1) {
        fprintf(stderr, "failed to call shmat(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    for (xfrs = 0, bytes = 0;; xfrs++) {
        if (reserveSem(semid, READ_SEM) == -1) {
            fprintf(stderr, "failed to call reservSem(), %d\n", errno);
            exit(EXIT_FAILURE);
        }

        if (shmp->cnt == 0) {
            break;
        }
        bytes += shmp->cnt;

        if (write(STDOUT_FILENO, shmp->buf, shmp->cnt) != shmp->cnt) {
            fprintf(stderr, "failed to call write(), %d\n", errno);
            exit(EXIT_FAILURE);
        }

        if (releaseSem(semid, WRITE_SEM) == -1) {
            fprintf(stderr, "failed to call releaseSem(), %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }

    if (shmdt(shmp) == -1) {
        fprintf(stderr, "failed to call shmdt(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (releaseSem(semid, WRITE_SEM) == -1) {
        fprintf(stderr, "failed to call releaseSem(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Received %d bytes (%d xfrs)\n", bytes, xfrs);
    return (EXIT_SUCCESS);
}