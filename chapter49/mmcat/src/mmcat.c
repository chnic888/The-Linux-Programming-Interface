#include "mmcat.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int mmcat(int argc, char *argv[]) {
    char *addr;
    int fd;
    struct stat sb;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s file\n", argv[1]);
        return (EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "failed to call open(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (fstat(fd, &sb) == -1) {
        fprintf(stderr, "failed to call fstat(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (sb.st_size == 0) {
        exit(EXIT_SUCCESS);
    }

    addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        fprintf(stderr, "failed to call mmap(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (write(STDOUT_FILENO, addr, sb.st_size) != sb.st_size) {
        fprintf(stderr, "failed to call write(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}