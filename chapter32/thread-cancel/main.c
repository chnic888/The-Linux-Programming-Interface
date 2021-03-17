#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

static void *threadFunc(void *arg) {
    int j;
    fprintf(stdout, "New thread started\n");
    for (j = 1;; j++) {
        fprintf(stdout, "Loop %d\n", j);
        sleep(1);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thr;
    int s;
    void *res;

    s = pthread_create(&thr, NULL, threadFunc, NULL);
    if (s != 0) {
        fprintf(stderr, "failed to call pthread_create(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    sleep(3);

    s = pthread_cancel(thr);
    if (s != 0) {
        fprintf(stderr, "failed to call pthread_cancel(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    s = pthread_join(thr, &res);
    if (s != 0) {
        fprintf(stderr, "failed to call pthread_join(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (res == PTHREAD_CANCELED) {
        fprintf(stdout, "Thread was canceled\n");
    } else {
        fprintf(stdout, "Thread was not canceled (should not happen!)\n");
    }

    exit(EXIT_SUCCESS);
}