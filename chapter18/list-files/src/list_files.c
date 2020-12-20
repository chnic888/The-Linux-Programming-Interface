#include "list_files.h"
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

static void scanDir(const char *);

int listFiles(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s [dir...]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    if (argc == 1) {
        scanDir(".");
    } else {
        for(argv++; *argv; argv++) {
            scanDir(*argv);
        }
    }

    return (EXIT_SUCCESS);
}

static void scanDir(const char *dirpath) {
    DIR *dirp;
    struct dirent *dp;
    bool isCurrent;

    isCurrent = strcmp(dirpath, ".") == 0;

    dirp = opendir(dirpath);
    if (dirp == NULL) {
        fprintf(stderr, "opendir failed on '%s'", dirpath);
        return;
    }

    for (;;) {
        errno = 0;
        dp = readdir(dirp);
        if (dp == NULL) {
            break;
        }

        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        if (!isCurrent) {
            fprintf(stdout, "%s/", dirpath);
        }
        fprintf(stdout, "%s\n", dp->d_name);
    }

    if (errno != 0) {
        fprintf(stderr, "failed to call readdir(), %d\n", errno);
        return;
    }

    if (closedir(dirp) == -1) {
        fprintf(stderr, "failed to call closedir(), %d\n", errno);
        return;
    }
}