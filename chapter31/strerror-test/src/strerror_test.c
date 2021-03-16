#include "strerror_test.h"

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>

static void *threadFunc(void *arg) {
    char *str;
    fprintf(stdout, "Other thread about to call strerror()\n");
    str = strerror(EPERM);
    fprintf(stdout, "Other thread: str (%p) = %s\n", str, str);
}

int strerrorTest(int argc, char *argv[]) {
    pthread_t t;
    int s;
    char *str;

    str = strerror(EINVAL);
    fprintf(stdout, "Main thread has called strerror()\n");

    s = pthread_create(&t, NULL, threadFunc, NULL);
    if (s != 0) {
        fprintf(stderr, "failed to call pthread_create(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    s = pthread_join(t, NULL);
    if (s != 0) {
        fprintf(stderr, "failed to call pthread_join(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "Main thread: str (%p) = %s\n", str, str);
    exit(EXIT_SUCCESS);
}