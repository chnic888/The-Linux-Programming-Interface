#include "svsem_create.h"
#include <sys/types.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

int svsemCreate(int argc, char *argv[]) {
    int opt, semid;
    int nsems = 0;
    char *endPtr = NULL;
    key_t key;

    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
            case 'p':
                key = IPC_PRIVATE;
                nsems = strtol(optarg, &endPtr, 10);
                break;
            default:
                fprintf(stderr, "%s Bad options \n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    semid = semget(key, nsems, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if (semid == -1) {
        fprintf(stderr, "failed to call semget(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "%d\n", semid);
    return (EXIT_SUCCESS);
}