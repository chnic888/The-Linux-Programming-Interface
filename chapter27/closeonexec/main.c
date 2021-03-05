#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    int flags;

    if (argc > 1) {
        flags = fcntl(STDOUT_FILENO, F_GETFD);
        if (flags == -1) {
            fprintf(stderr, "failed to call fcntl(F_GETFD), %d\n", errno);
            return (EXIT_FAILURE);
        }

        flags |= FD_CLOEXEC;

        if (fcntl(STDOUT_FILENO, F_SETFD, flags) == -1) {
            fprintf(stderr, "failed to call fcntl(F_SETFD), %d\n", errno);
            return (EXIT_FAILURE);
        }
    }

    execlp("ls", "ls", "-l", argv[0], (char *) NULL);

    fprintf(stderr, "failed to call execlp(), %d\n", errno);
    exit(EXIT_FAILURE);
}