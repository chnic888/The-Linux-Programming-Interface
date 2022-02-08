#include "anon_mmap.h"

#ifdef USE_MAP_ANON
#define _BSD_SOURCE
#endif

#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int anonMmap(int argc, char *argv[]) {
    int *addr;

#ifdef USE_MAP_ANON
    addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED) {
        fprintf(stderr, "failed to call mmap(), %d\n", errno);
        return (EXIT_FAILURE);
    }

#else
    int fd;

    fd = open("/dev/zero", O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "failed to call open(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        fprintf(stderr, "failed to call mmap(), %d\n", errno);
        return (EXIT_FAILURE);
    }

   if (close(fd) == -1) {
       fprintf(stderr, "failed to call close(), %d\n", errno);
       return (EXIT_FAILURE);
   }
#endif

    *addr = 1;

    switch (fork()) {
        case -1:
            fprintf(stderr, "failed to call fork(), %d\n", errno);
            break;
        case 0:
            fprintf(stdout, "Child started, value = %d\n", *addr);
            (*addr)++;

            if (munmap(addr, sizeof(int)) == -1) {
                fprintf(stderr, "failed to call munmap(), %d\n", errno);
                exit(EXIT_FAILURE);
            }

            exit(EXIT_SUCCESS);
        default:
            if (wait(NULL) == -1) {
                fprintf(stderr, "failed to call wait(), %d\n", errno);
                exit(EXIT_FAILURE);
            }

            fprintf(stdout, "In parent, value = %d\n", *addr);

            if (munmap(addr, sizeof(int)) == -1) {
                fprintf(stderr, "failed to call munmap(), %d\n", errno);
                exit(EXIT_FAILURE);
            }
            exit(EXIT_SUCCESS);
    }

    return (EXIT_SUCCESS);
}