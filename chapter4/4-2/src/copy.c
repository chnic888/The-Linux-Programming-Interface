#include <fcntl.h>
#include <unistd.h>
#include "copy.h"

#define MAX_BUF_SIZE 1024

int copy(int argc, char *argv[]) {
    int in, out;
    char buf[1024];
    ssize_t numRead;

    in = open(argv[1], O_RDONLY);
    if (in == -1) {
        fprintf(stderr, "failed to open file %s, %d\n", argv[1], errno);
        return (EXIT_FAILURE);
    }

    out = open(argv[2], O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (out == -1) {
        fprintf(stderr, "failed to create file %s, %d\n", argv[2], errno);
        return (EXIT_FAILURE);
    }

    while ((numRead = read(in, buf, MAX_BUF_SIZE)) > 0) {
        if (write(out, buf, numRead) != numRead) {
            fprintf(stderr, "failed to write %d\n", errno);
            return (EXIT_FAILURE);
        }
    }

    if (numRead == -1) {
        fprintf(stderr, "failed to read %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (close(in) == -1) {
        fprintf(stderr, "failed to close source file %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (close(out) == -1) {
        fprintf(stderr, "failed to close target file %d\n", errno);
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}