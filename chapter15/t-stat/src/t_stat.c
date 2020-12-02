#include "t_stat.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

static void displayStatInfo(const struct stat *);

int tStat(int argc, char *argv[]) {
    struct stat sb;
    bool statLink;
    int fname;

    statLink = (argc > 1) && strcmp(argv[1], "-l") == 0;
    fname = statLink ? 2 : 1;

    if (fname >= argc || (argc > 1 && strcmp(argv[1], "--help") == 0)) {
        fprintf(stderr, "%s [-l] file\n"
                        "           -l = use lstat() instead of stat()\n", argv[0]);
        return (EXIT_FAILURE);
    }

    if (statLink) {
        if (lstat(argv[fname], &sb) == -1) {
            fprintf(stderr, "failed to call lstat(), %d\n", errno);
            return (EXIT_FAILURE);
        }
    } else {
        if (stat(argv[fname], &sb) == -1) {
            fprintf(stderr, "failed to call stat(), %d\n", errno);
            return (EXIT_FAILURE);
        }
    }

    displayStatInfo(&sb);
    return (EXIT_SUCCESS);
}

static void displayStatInfo(const struct stat *sb) {
    fprintf(stdout, "File type:     ");

    switch (sb->st_mode & S_IFMT) {
        case S_IFREG:
            fprintf(stdout, "regular file\n");
            break;
        case S_IFDIR:
            fprintf(stdout, "directory\n");
            break;
        case S_IFCHR:
            fprintf(stdout, "character device\n");
            break;
        case S_IFBLK:
            fprintf(stdout, "block device\n");
            break;
        case S_IFLNK:
            fprintf(stdout, "symbolic (soft) link\n");
            break;
        case S_IFIFO:
            fprintf(stdout, "FIFO or pipe\n");
            break;
        case S_IFSOCK:
            fprintf(stdout, "socket\n");
            break;
        default:
            fprintf(stdout, "unknown file type?\n");
            break;
    }

    fprintf(stdout, "Device containing i-node: major=%ld minor=%ld\n", (long) major(sb->st_dev),
            (long) minor(sb->st_dev));
    fprintf(stdout, "I-node number: %ld\n", (long) sb->st_ino);

//    fprintf(stdout, "Mode: %lo (%s)\n", (unsigned long) sb->st_mode, filePermStr(sb->st_mode, 0));

    if (sb->st_mode & (S_ISUID | S_ISGID | S_ISVTX)) {
        fprintf(stdout, " special bits set: %s%s%s\n", (sb->st_mode & S_ISUID) ? "set-UID " : "",
                (sb->st_mode & S_ISGID) ? "set-GID " : "", (sb->st_mode & S_ISVTX) ? "sticky " : "");
    }

    fprintf(stdout, "Number of (hard) links: %ld\n", (long) sb->st_nlink);
    fprintf(stdout, "Ownership: UID=%ld GID=%ld\n", (long) sb->st_uid, (long) sb->st_gid);

    if (S_ISCHR(sb->st_mode) || S_ISBLK(sb->st_mode)) {
        fprintf(stdout, "Device number (st_rdev): major=%ld; minor=%ld\n", (long) major(sb->st_rdev),
                (long) minor(sb->st_rdev));
    }

    fprintf(stdout, "File size: %lld bytes\n", (long long) sb->st_size);
    fprintf(stdout, "Optimal I/O block size: %ld bytes\n", (long) sb->st_blksize);
    fprintf(stdout, "512B blocks allocated: %lld\n", (long long) sb->st_blocks);

    const long *at = &sb->st_atime;
    fprintf(stdout, "Last file access: %ld, %s", *at, ctime(at));

    const long *mt = &sb->st_mtime;
    fprintf(stdout, "Last file modification: %ld, %s", *mt, ctime(mt));

    const long *ct = &sb->st_ctime;
    fprintf(stdout, "Last status change: %ld, %s", *ct, ctime(ct));
}