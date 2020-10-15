#include "writev.h"
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#define IOV_SIZE 8

int wrv(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s filename", argv[0]);
        return (EXIT_FAILURE);
    }

    struct player messi = {
            .firstName = "Lionel",
            .lastName = "Messi",
            33,
            .league = "La Liga",
            .club = "FC Barcelona",
            .rating = 5,
            .point = 7,
    };

    struct player ronaldo = {
            .firstName = "Cristiano",
            .lastName = "Ronaldo",
            .age = 35,
            .league = "Serie A",
            .club = "Juventus F.C.",
            .rating = 8,
            .point = 2,
    };

    struct player players[2] = {
            messi,
            ronaldo
    };

    int fd = open(argv[1], O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        fprintf(stderr, "failed to open file %s, %d\n", argv[1], errno);
        return (EXIT_FAILURE);
    }

    struct iovec iov[IOV_SIZE];
    char lineBreak = '\n';

    for (int i = 0; i < sizeof(players) / sizeof(players[0]); i++) {
        size_t totalSize = 0;
        struct player p = players[i];

        iov[0].iov_base = p.firstName;
        iov[0].iov_len = SIZE;
        totalSize += iov[0].iov_len;

        iov[1].iov_base = p.lastName;
        iov[1].iov_len = SIZE;
        totalSize += iov[1].iov_len;

        iov[2].iov_base = &p.age;
        iov[2].iov_len = sizeof(p.age);
        totalSize += iov[2].iov_len;

        iov[3].iov_base = p.league;
        iov[3].iov_len = SIZE;
        totalSize += iov[3].iov_len;

        iov[4].iov_base = p.club;
        iov[4].iov_len = SIZE;
        totalSize += iov[4].iov_len;

        iov[5].iov_base = &p.rating;
        iov[5].iov_len = sizeof(p.rating);
        totalSize += iov[5].iov_len;

        iov[6].iov_base = &p.point;
        iov[6].iov_len = sizeof(p.point);
        totalSize += iov[6].iov_len;

        iov[7].iov_base = &lineBreak;
        iov[7].iov_len = sizeof(lineBreak);
        totalSize += iov[7].iov_len;

        if (writev(fd, iov, IOV_SIZE) != totalSize) {
            fprintf(stderr, "failed to write file %s, %d\n", argv[1], errno);
            return (EXIT_FAILURE);
        }
    }

    if (close(fd) == -1) {
        fprintf(stderr, "failed to close file, %d", errno);
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}