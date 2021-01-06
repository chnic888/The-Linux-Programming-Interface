#define _XOPEN_SOURCE 600

#include "nftw_dir_tree.h"
#include <stdlib.h>
#include <stdio.h>
#include <ftw.h>
#include <string.h>
#include <unistd.h>

static void usageError(const char *, const char *);

static int dirTree(const char *, const struct stat *, int, struct FTW *);

int nftwDirTree(int argc, char *argv[]) {
    int flags, opt;

    flags = 0;
    while ((opt = getopt(argc, argv, "dmp")) != -1) {
        switch (opt) {
            case 'd':
                flags |= FTW_DEPTH;
                break;
            case 'm':
                flags |= FTW_MOUNT;
                break;
            case 'p':
                flags |= FTW_PHYS;
                break;
            default:
                usageError(argv[0], NULL);
        }
    }

    if (argc > optind + 1)
        usageError(argv[0], NULL);

    if (nftw((argc > optind) ? argv[optind] : ".", dirTree, 10, flags) == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

static int dirTree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb) {
    //check and print file type
    switch (sbuf->st_mode & S_IFMT) {
        case S_IFREG:
            fprintf(stdout, "-");
            break;
        case S_IFDIR:
            fprintf(stdout, "d");
            break;
        case S_IFCHR:
            fprintf(stdout, "c");
            break;
        case S_IFBLK:
            fprintf(stdout, "b");
            break;
        case S_IFLNK:
            fprintf(stdout, "l");
            break;
        case S_IFIFO:
            fprintf(stdout, "p");
            break;
        case S_IFSOCK:
            fprintf(stdout, "s");
            break;
        default:
            fprintf(stdout, "-");
            break;
    }

    printf(" %s ",
           (type == FTW_D) ? "D " : (type == FTW_DNR) ? "DNR" :
                                    (type == FTW_DP) ? "DP " : (type == FTW_F) ? "F " :
                                                               (type == FTW_SL) ? "SL " : (type == FTW_SLN) ? "SLN" :
                                                                                          (type == FTW_NS) ? "NS "
                                                                                                           : " ");

    if (type != FTW_NS)
        fprintf(stdout, "%7ld", (long) sbuf->st_ino);
    else
        fprintf(stdout, "    ");

    fprintf(stdout, " %*s", 4 * ftwb->level, "");
    fprintf(stdout, "%s\n", &pathname[ftwb->base]);

    return (EXIT_SUCCESS);
}

static void usageError(const char *progName, const char *msg) {
    if (msg != NULL)
        fprintf(stderr, "%s/n", msg);

    fprintf(stderr, "Usage %s [-d] [-m] [-p] [directory-path]\n", progName);
    fprintf(stderr, "\t-d Use FTW_DEPTH flag\n");
    fprintf(stderr, "\t-m Use FTW_MOUNT flag\n");
    fprintf(stderr, "\t-p Use FTW_PHYS flag\n");

    exit(EXIT_FAILURE);
}