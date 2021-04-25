#include <stdio.h>
#include <lastlog.h>
#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>

int main(int argc, char *argv[]) {
    struct lastlog llog;
    int fd, j;
    struct passwd *pwd = NULL;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s [username...]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    fd = open(_PATH_LASTLOG, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "failed to open lastlog file, %d\n", errno);
        exit(EXIT_FAILURE);
    }

    for (j = 1; j < argc; j++) {
        pwd = getpwnam(argv[j]);
        if (pwd == NULL) {
            fprintf(stderr, "No such user: %s\n", argv[j]);
            continue;
        }

        if (lseek(fd, pwd->pw_uid * sizeof(struct lastlog), SEEK_SET) == -1) {
            fprintf(stderr, "failed to seek lastlog file, %d\n", errno);
            exit(EXIT_FAILURE);
        }

        if (read(fd, &llog, sizeof(struct lastlog)) <= 0) {
            fprintf(stderr, "read failed for %s\n", argv[j]);
            continue;
        }

        int32_t lt = llog.ll_time;
        fprintf(stdout, "%-8.8s %-6.6s %-20.20s %s", argv[j], llog.ll_line, llog.ll_host,  ctime((const time_t *) &lt));
    }

    exit(EXIT_SUCCESS);
}