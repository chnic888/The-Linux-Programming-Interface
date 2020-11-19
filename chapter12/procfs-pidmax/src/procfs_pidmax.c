#include "procfs_pidmax.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINE 100

int procfsPidmax(int argc, char *argv[]) {
    int fd;
    char line[MAX_LINE];
    ssize_t n;

    fd = open("/proc/sys/kernel/pid_max", argc > 1 ? O_RDWR : O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "failed to open file, %d\n", errno);
        return (EXIT_FAILURE);
    }

    n = read(fd, line, MAX_LINE);
    if (n == -1) {
        fprintf(stderr, "failed to read file, %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (argc > 1) {
        fprintf(stdout, "Old value: ");
    }
    fprintf(stdout, "%.*s", (int) n, line);

    if (argc > 1) {
        if (write(fd, argv[1], strlen(argv[1])) != strlen(argv[1])) {
            fprintf(stderr, "failed to write file, %d\n", errno);
            return (EXIT_FAILURE);
        }
        system("echo /proc/sys/kernel/pid_max now contains `cat /proc/sys/kernel/pid_max`\n");
    }

    return (EXIT_SUCCESS);
}