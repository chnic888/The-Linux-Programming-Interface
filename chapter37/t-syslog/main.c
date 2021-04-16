#include <syslog.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    openlog(argv[0], LOG_PID | LOG_CONS, LOG_LOCAL0);
    syslog(LOG_ERR, "I am ready");
    syslog(LOG_USER | LOG_INFO, "WHO AM I");
    closelog();

    fprintf(stdout, "finished\n");
    return (EXIT_SUCCESS);
}
