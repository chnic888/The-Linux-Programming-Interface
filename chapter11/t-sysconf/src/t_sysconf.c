#include "t_sysconf.h"
#include <errno.h>
#include <unistd.h>

static void sysconfPrint(const char *, int);

int tSysconf(int argc, char *argv[]) {
    sysconfPrint("_SC_ARG_MAX: ", _SC_ARG_MAX);
    sysconfPrint("_SC_LOGIN_NAME_MAX: ", _SC_LOGIN_NAME_MAX);
    sysconfPrint("_SC_OPEN_MAX: ", _SC_OPEN_MAX);
    sysconfPrint("_SC_NGROUPS_MAX: ", _SC_NGROUPS_MAX);
    sysconfPrint("_SC_PAGESIZE: ", _SC_PAGESIZE);
    sysconfPrint("_SC_RTSIG_MAX: ", _SC_RTSIG_MAX);

    return (EXIT_SUCCESS);
}

static void sysconfPrint(const char *msg, int name) {
    long lim;

    errno = 0;
    lim = sysconf(name);

    if (lim != -1) {
        fprintf(stderr, "%s %ld\n", msg, lim);
        return;
    }

    if (errno == 0) {
        fprintf(stdout, "%s (indeterminate)\n", msg);
        return;
    }

    fprintf(stderr, "sysconf %s", msg);
}