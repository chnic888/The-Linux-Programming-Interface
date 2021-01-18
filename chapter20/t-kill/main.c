#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    int sig, s;
    char *endPtr = NULL;

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s sig-num pid\n", argv[0]);
        return (EXIT_FAILURE);
    }

    sig = (int) strtol(argv[2], &endPtr, 10);
    endPtr = NULL;

    long pid = strtol(argv[1], &endPtr, 10);
    s = kill(pid, sig);

    if (sig != 0 ) {
        if (s == -1) {
            fprintf(stderr, "filed to call kill(), %d\n", errno);
            return (EXIT_FAILURE);
        }
    } else {
        if (s == 0) {
            fprintf(stdout, "Process exists and we can send it a signal\n");
        } else if (errno == ESRCH) {
            fprintf(stderr, "Process does not exist\n");
        } else {
            fprintf(stderr, "filed to call kill(), %d\n", errno);
            return (EXIT_FAILURE);
        }
    }

    return (EXIT_SUCCESS);
}