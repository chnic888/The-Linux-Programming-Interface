#include "t_umask.h"
#include "file_perms.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define MYFILE "myfile"
#define MYDIR "mydir"
#define FILE_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define DIR_PERMS (S_IRWXU | S_IRWXG | S_IRWXO)
#define UMASK_SETTING (S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH)

int tUmask(int argc, char *argv[]) {
    int fd;
    struct stat sb;
    mode_t u;

    umask(UMASK_SETTING);
    fd = open(MYFILE, O_RDWR | O_CREAT | O_EXCL, FILE_PERMS);
    if (fd == -1) {
        fprintf(stderr, "failed to open file %s, %d\n", MYFILE, errno);
        return (EXIT_FAILURE);
    }

    if (mkdir(MYDIR, DIR_PERMS) == -1) {
        fprintf(stderr, "failed create dir %s, %d\n", MYDIR, errno);
        return (EXIT_FAILURE);
    }

    u = umask(0);

    if (stat(MYFILE, &sb) == -1) {
        fprintf(stderr, "failed to call stat() %s, %d\n", MYFILE, errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Requested file perms: %s\n", filePermStr(FILE_PERMS, 0));
    fprintf(stdout, "Process umask: %s\n", filePermStr(u, 0));
    fprintf(stdout, "Actual file perms: %s\n\n", filePermStr(sb.st_mode, 0));

    if (stat(MYDIR, &sb) == -1) {
        fprintf(stderr, "failed to call stat() %s, %d\n", MYDIR, errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Requested dir. perms: %s\n", filePermStr(DIR_PERMS, 0));
    fprintf(stdout, "Process umask: %s\n", filePermStr(u, 0));
    fprintf(stdout, "Actual dir. perms: %s\n", filePermStr(sb.st_mode, 0));

    if (unlink(MYFILE) == -1) {
        fprintf(stderr, "failed to unlink %s, %d\n", MYFILE, errno);
        return (EXIT_FAILURE);
    }

    if (rmdir(MYDIR) == -1) {
        fprintf(stderr, "failed to rmdir %s, %d\n", MYDIR, errno);
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}