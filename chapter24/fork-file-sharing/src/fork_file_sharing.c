#include "fork_file_sharing.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

int forkFileSharing(int argc, char *argv[]) {
    int fd, flags;
    char template[] = "/tmp/testXXXXXX";

    setbuf(stdout, NULL);

    fd = mkstemp(template);
    if (fd == -1) {
        fprintf(stderr, "failed to call mkstemp(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "File offset before fork(): %lld\n", (long long) lseek(fd, 0, SEEK_CUR));

    flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        fprintf(stderr, "failed to call fcntl(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "O_APPEND flag before fork() is: %s\n", (flags & O_APPEND) ? "on" : "off");

    switch (fork()) {
        case -1:
            fprintf(stderr, "failed to call fork(), %d\n", errno);
            exit(EXIT_FAILURE);
        case 0:
            if (lseek(fd, 1000, SEEK_CUR) == -1) {
                fprintf(stderr, "failed to call lseek(), %d\n", errno);
                exit(EXIT_FAILURE);
            }

            flags = fcntl(fd, F_GETFL);
            if (flags == -1) {
                fprintf(stderr, "failed to call fcntl(), %d\n", errno);
                exit(EXIT_FAILURE);
            }

            flags |= O_APPEND;
            if (fcntl(fd, F_SETFL, flags) == -1) {
                fprintf(stderr, "failed to call fcntl(), %d\n", errno);
                exit(EXIT_FAILURE);
            }

            _exit(EXIT_SUCCESS);
        default:
            if (wait(NULL) == -1) {
                fprintf(stderr, "failed to call wait(), %d\n", errno);
                exit(EXIT_FAILURE);
            }

            fprintf(stdout, "Child has exited\n");
            fprintf(stdout, "File offset in parent: %lld\n", (long long) lseek(fd, 0, SEEK_CUR));

            flags = fcntl(fd, F_GETFL);
            if (flags == -1) {
                fprintf(stderr, "failed to call fcntl(), %d\n", errno);
                exit(EXIT_FAILURE);
            }

            fprintf(stdout, "O_APPEND flag in parent is: %s\n", (flags & O_APPEND) ? "on" : "off");
            exit(EXIT_SUCCESS);
    }
}