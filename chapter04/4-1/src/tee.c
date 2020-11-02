#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "tee.h"

#define MAX_SIZE 100

char *getTeeOpt(int, char *[], _Bool *);

int tee(int argc, char *argv[]) {
    int numberRead;
    char buf[MAX_SIZE];
    _Bool isAppend = false;
    int outputFd;

    char *fileName = getTeeOpt(argc, argv, &isAppend);
    if (fileName == NULL) {
        fprintf(stderr, "failed to get file name\n");
        return (EXIT_FAILURE);
    }

    if (isAppend && access(fileName, F_OK) != -1) {
        outputFd = open(fileName, O_WRONLY | O_APPEND);
    } else {
        outputFd = open(fileName, O_CREAT | O_WRONLY | O_TRUNC,
                        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    }

    while ((numberRead = read(fileno(stdin), buf, MAX_SIZE)) > 0) {
        char tmpBuf[MAX_SIZE];
        strcpy(tmpBuf, buf);

        if (write(outputFd, buf, numberRead) != numberRead) {
            fprintf(stderr, "%d, %d\n", errno, numberRead);
        }

        if (write(fileno(stdout), tmpBuf, numberRead) != numberRead) {
            fprintf(stderr, "%d, %d\n", errno, numberRead);
        }
    }

    if (close(outputFd) == -1) {
        fprintf(stderr, "failed to close output file\n");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

char *getTeeOpt(int argc, char *argv[], _Bool *isAppend) {
    int ch;
    if ((ch = getopt(argc, argv, "a:")) != -1) {
        if (ch == 'a') {
            *isAppend = true;
            return optarg;
        } else {
            fprintf(stderr, "error argument %s\n", optarg);
        }
    } else {
        return argv[1];
    }

    return NULL;
}
