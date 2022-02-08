#include "t_map.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define MEM_SIZE 10

int tMap(int argc, char *argv[]) {
    char *addr;
    int fd;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stdout, "%s file [new-value]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "failed to call open(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    addr = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        fprintf(stderr, "failed to call mmap(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        fprintf(stderr, "failed to call close(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Current string=%.*s\n", MEM_SIZE, addr);

    if (argc > 2) {
        if (strlen(argv[2]) >= MEM_SIZE) {
            fprintf(stdout, "new-value' too large\n");
            return (EXIT_FAILURE);
        }

        memset(addr, 0, MEM_SIZE);
        strncpy(addr, argv[2], MEM_SIZE - 1);
        if (msync(addr, MEM_SIZE, MS_SYNC) == -1) {
            fprintf(stderr, "failed to call msync(), %d\n", errno);
            return (EXIT_FAILURE);
        }

        fprintf(stderr, "Copied \"%s\" to shared memory\n", argv[2]);
    }

    return (EXIT_SUCCESS);
}