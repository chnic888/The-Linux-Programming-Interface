#define _GNU_SOURCE

#include <stdio.h>
#include <utmpx.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

int main(int argc, char *argv[]) {
    struct utmpx *ut;

    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s [utmp-pathname]\n", argv[0]);
        return (EXIT_FAILURE);
    }

    if (argc > 1) {
        if (utmpxname(argv[1]) == -1) {
            fprintf(stderr, "failed to call utmpxname(), %d", errno);
            exit(EXIT_FAILURE);
        }
    }

    setutxent();

    fprintf(stdout, "user       type        PID line        id host         date/time\n");

    while ((ut = getutxent()) != NULL) {
        fprintf(stdout, "%-8s ", ut->ut_user);
        fprintf(stdout, "%-9.9s ",
                (ut->ut_type == EMPTY) ? "EMPTY" :
                (ut->ut_type == RUN_LVL) ? "RUN_LVL" :
                (ut->ut_type == BOOT_TIME) ? "BOOT_TIME" :
                (ut->ut_type == NEW_TIME) ? "NEW_TIME" :
                (ut->ut_type == OLD_TIME) ? "OLD_TIME" :
                (ut->ut_type == INIT_PROCESS) ? "INIT_PR" :
                (ut->ut_type == LOGIN_PROCESS) ? "LOGIN_PR" :
                (ut->ut_type == USER_PROCESS) ? "USER_PR" :
                (ut->ut_type == DEAD_PROCESS) ? "DEAD_PR" : "???");
        fprintf(stdout, "%5ld %-6.6s %-3.5s %-9.9s", (long) ut->ut_pid, ut->ut_line, ut->ut_id, ut->ut_host);
        fprintf(stdout, "   %s", ctime((time_t *) &(ut->ut_tv.tv_sec)));
    }

    endutxent();
    exit(EXIT_SUCCESS);
}