#include "readv.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/uio.h>
#include <unistd.h>

#define IOV_SIZE 8

int rdv(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s filename", argv[0]);
        return (EXIT_FAILURE);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "failed to open file %s\n, %d", argv[1], errno);
        return (EXIT_FAILURE);
    }

    char lineBreak;
    struct player p;
    struct iovec iov[IOV_SIZE];

    iov[0].iov_base = p.firstName;
    iov[0].iov_len = SIZE;

    iov[1].iov_base = p.lastName;
    iov[1].iov_len = SIZE;

    iov[2].iov_base = &p.age;
    iov[2].iov_len = sizeof(p.age);

    iov[3].iov_base = p.league;
    iov[3].iov_len = SIZE;

    iov[4].iov_base = p.club;
    iov[4].iov_len = SIZE;

    iov[5].iov_base = &p.rating;
    iov[5].iov_len = sizeof(p.rating);

    iov[6].iov_base = &p.point;
    iov[6].iov_len = sizeof(p.point);

    iov[7].iov_base = &lineBreak;
    iov[7].iov_len = sizeof(lineBreak);

    for (int i = 0; i < 2; i++) {
        ssize_t readNum = readv(fd, iov, IOV_SIZE);
        if (readNum == -1) {
            fprintf(stderr, "failed to read file %s, %d", argv[1], errno);
            return (EXIT_FAILURE);
        }

        fprintf(stdout, "name: %s %s\n", p.firstName, p.lastName);
        fprintf(stdout, "age: %d\n", p.age);
        fprintf(stdout, "league: %s, club: %s\n", p.league, p.club);
        fprintf(stdout, "rating %d, point %d\n", p.rating, p.point);
        printf("\n");
    }

    if (close(fd) == -1) {
        fprintf(stderr, "failed to close file\n");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}