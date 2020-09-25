#include "large_file.h"
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define _FILE_OFFSET_BITS 64

int largeFile(int argc, char *argv[]) {
    int fd;
    off_t offset;

    if (argc != 3 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s pathname offset\n", argv[0]);
        return (EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        fprintf(stderr, "failed to create file %s", argv[1]);
        return (EXIT_FAILURE);
    }

    char *end = argv[2] + strlen(argv[2]);
    offset = strtoll(argv[2], &end, 0);

    if (lseek(fd, offset, SEEK_SET) == -1) {
        fprintf(stderr, "failed to seek");
        return (EXIT_FAILURE);
    }

    if (write(fd, "test", 4) == -1) {
        fprintf(stderr, "failed to write string");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}