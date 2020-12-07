#include "t_statvfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/statvfs.h>

int tStatvfs(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s pathname\n", argv[0]);
        return (EXIT_FAILURE);
    }

    struct statvfs sv;
    if (statvfs(argv[1], &sv) == -1) {
        fprintf(stderr, "failed to call statvfs(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "File-system block size                                     %ld\n", sv.f_bsize);
    fprintf(stdout, "Fundamental file-system block size                         %ld\n", sv.f_frsize);
    fprintf(stdout, "Total number of blocks in file system                      %ld\n", sv.f_blocks);
    fprintf(stdout, "Total number of free blocks                                %ld\n", sv.f_bfree);
    fprintf(stdout, "Number of free blocks available to unprivileged process    %ld\n", sv.f_bavail);
    fprintf(stdout, "Total number of i-nodes                                    %ld\n", sv.f_files);
    fprintf(stdout, "Total number of free i-nodes                               %ld\n", sv.f_ffree);
    fprintf(stdout, "Number of i-nodes available to unprivileged process        %ld\n", sv.f_favail);
    fprintf(stdout, "File-system ID                                             %ld\n", sv.f_fsid);
    fprintf(stdout, "Mount flags                                                %ld\n", sv.f_flag);
    fprintf(stdout, "Maximum length of filenames on this file system            %ld\n", sv.f_namemax);

    return (EXIT_SUCCESS);
}
