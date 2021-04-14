#include "print_rlimit.h"
#include <sys/resource.h>
#include <stdio.h>

int printRlimit(const char *msg, int resource) {
    struct rlimit rlim;

    if (getrlimit(resource, &rlim) == -1) {
        return -1;
    }

    fprintf(stdout, "%s soft=", msg);
    if (rlim.rlim_cur == RLIM_INFINITY)
        fprintf(stdout, "infinite");
#ifdef RLIM_SAVED_CUR
    else if (rlim.rlim_cur == RLIM_SAVED_CUR)
        fprintf(stdout, "unrepresentable");
#endif
    else
        fprintf(stdout, "%lld", (long long) rlim.rlim_cur);

    fprintf(stdout, "; hard=");
    if (rlim.rlim_max == RLIM_INFINITY)
        fprintf(stdout, "infinite\n");
#ifdef RLIM_SAVED_MAX
    else if (rlim.rlim_max == RLIM_SAVED_MAX)
        fprintf(stdout, "unrepresentable");
#endif
    else
        fprintf(stdout, "%lld\n", (long long) rlim.rlim_max);

    return 0;
}