#include "svshm_xfr_writer.h"
#include "semun.h"
#include "svshm_xfr.h"

int svshmXfrWriter(int argc, char *argv[]) {
    int semid, shmid, bytes, xfrs;
    struct shmseg *shmp;
    union semun dummy;

    semid = semget(SEM_KEY, 2, IPC_CREAT | OBJ_PERMS);
    if (semid == -1) {
        fprintf(stderr, "failed to call semget(), %d\n", errno);
        return (EXIT_FAILURE);
    }
    fprintf(stdout, "semid = %d\n", semid);

    if (initSemAvailable(semid, WRITE_SEM) == -1) {
        fprintf(stderr, "failed to call initSemAvailable(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (initSemInUse(semid, READ_SEM) == -1) {
        fprintf(stderr, "failed to call initSemInUse(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    shmid = shmget(SHM_KEY, sizeof(struct shmseg), IPC_CREAT | OBJ_PERMS);
    if (shmid == -1) {
        fprintf(stderr, "failed to call shmget(), %d\n", errno);
        return (EXIT_FAILURE);
    }
    fprintf(stdout, "shmid = %d\n", shmid);

    shmp = shmat(shmid, NULL, 0);
    if (shmp == (void *) -1) {
        fprintf(stderr, "failed to call shmat(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    for (xfrs = 0, bytes = 0;; xfrs++, bytes += shmp->cnt) {
        if (reserveSem(semid, WRITE_SEM) == -1) {
            fprintf(stderr, "failed to call reserveSem(), %d\n", errno);
            exit(EXIT_FAILURE);
        }

        shmp->cnt = (int) read(STDIN_FILENO, shmp->buf, BUF_SIZE);
        if (shmp->cnt == -1) {
            fprintf(stderr, "failed to call read(), %d\n", errno);
            exit(EXIT_FAILURE);
        }

        if (releaseSem(semid, READ_SEM) == -1) {
            fprintf(stderr, "failed to call releaseSem(), %d\n", errno);
            exit(EXIT_FAILURE);
        }

        if (shmp->cnt == 0) {
            break;
        }
    }

    if (reserveSem(semid, WRITE_SEM) == -1) {
        fprintf(stderr, "failed to call reserveSem(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (shmdt(shmp) == -1) {
        fprintf(stderr, "failed to call shmdt(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "failed to call shmctl(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stderr, "Send %d bytes (%d xfrs)\n", bytes, xfrs);
    return (EXIT_SUCCESS);
}