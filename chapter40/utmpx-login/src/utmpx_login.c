#define _GNU_SOURCE

#include "utmpx_login.h"
#include <time.h>
#include <utmpx.h>
#include <paths.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int utmpxLogin(int argc, char *argv[]) {
    struct utmpx ut;
    char *devName;
    char *endPtr = NULL;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s username [sleep-time]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    memset(&ut, 0, sizeof(struct utmpx));
    ut.ut_type = USER_PROCESS;
    strncpy(ut.ut_user, argv[1], sizeof(ut.ut_user));

    if (time((time_t *) &ut.ut_tv.tv_sec) == -1) {
        fprintf(stderr, "failed to get time(), %d\n", errno);
        exit(EXIT_FAILURE);
    }
    ut.ut_pid = getpid();

    devName = ttyname(STDIN_FILENO);
    if (devName == NULL) {
        fprintf(stderr, "failed to call ttyname, %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if (strlen(devName) <= 8) {
        fprintf(stderr, "Terminal name is too short: %s\n", devName);
        exit(EXIT_FAILURE);
    }

    strncpy(ut.ut_line, devName + 5, sizeof(ut.ut_line));
    strncpy(ut.ut_id, devName + 8, sizeof(ut.ut_id));

    fprintf(stdout, "Creating login entries in utmp and wtmp\n");
    fprintf(stdout, "   using pid %ld, line %.*s, id %.*s\n", (long) ut.ut_pid, (int) sizeof(ut.ut_line), ut.ut_line,
            (int) sizeof(ut.ut_id), ut.ut_id);

    setutxent();

    if (pututxline(&ut) == NULL) {
        fprintf(stderr, "pututxline\n");
    }

    updwtmpx(_PATH_WTMP, &ut);

    sleep((argc > 2) ? strtol(argv[2], &endPtr, 10) : 15);

    ut.ut_type = DEAD_PROCESS;
    time((time_t *) &ut.ut_tv.tv_sec);
    memset(&ut.ut_user, 0, sizeof(ut.ut_user));

    fprintf(stdout, "Creating logout entries in utmp and wtmp\n");
    setutxent();
    if (pututxline(&ut) == NULL) {
        fprintf(stderr, "failed to call pututxline(), %d\n", errno);
        exit(EXIT_FAILURE);
    }
    updwtmpx(_PATH_WTMP, &ut);

    endutxent();
    exit(EXIT_SUCCESS);

}