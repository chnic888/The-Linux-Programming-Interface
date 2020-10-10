#include "append_seek_set.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int appendSeekSet(int argc, char *argv[]) {
    int fd;

    if (argc != 2 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s pathname offset\n", argv[0]);
        return (EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDWR | O_APPEND);
    if (fd == -1) {
        fprintf(stderr, "failed to create file %s", argv[1]);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "current seek offset is %lld\n", lseek(fd, 0, SEEK_CUR));

    off_t offset = lseek(fd, 100000, SEEK_CUR);
    if (offset == -1) {
        fprintf(stderr, "failed to seek the beginning of the file %s", argv[1]);
        return (EXIT_FAILURE);
    }
    fprintf(stdout, "current seek offset is %lld\n", offset);

    char *str = "test";
    if (write(fd, str, strlen(str)) == -1) {
        fprintf(stderr, "failed to write data");
        return (EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        fprintf(stderr, "failed to close the file descriptor");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}
