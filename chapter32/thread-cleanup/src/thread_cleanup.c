#include "thread_cleanup.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static int glob = 0;

static void cleanupHandler(void *arg) {
    int s;

    fprintf(stdout, "cleanup: freeing block at %p\n", arg);
    free(arg);

    fprintf(stdout, "cleanup: unlocking mutex\n");
    s = pthread_mutex_unlock(&mtx);
    if (s != 0) {
        fprintf(stdout, "failed to call pthread_mutex_unlock(), %d\n", errno);
        return;
    }
}

static void *threadFunc(void *arg) {
    int s;
    void *buf = NULL;

    buf = malloc(0x10000);
    fprintf(stdout, "thread: allocated memory at %p\n", buf);

    s = pthread_mutex_lock(&mtx);
    if (s != 0) {
        fprintf(stdout, "failed to call pthread_mutex_lock(), %d\n", errno);
        return NULL;
    }

    pthread_cleanup_push(cleanupHandler, buf);

    while (glob == 0) {
        s = pthread_cond_wait(&cond, &mtx);
        if (s != 0) {
            fprintf(stdout, "failed to call pthread_cond_wait(), %d\n", errno);
            return NULL;
        }
    }

    fprintf(stdout, "thread: condition wait loop completed\n");
    pthread_cleanup_pop(1);
    return NULL;
}

int threadCleanup(int argc, char *argv[]) {
    pthread_t thr;
    void *res;
    int s;

    s = pthread_create(&thr, NULL, threadFunc, NULL);
    if (s != 0) {
        fprintf(stdout, "thread: condition wait loop completed\n");
        exit(EXIT_FAILURE);
    }

    sleep(2);

    if (argc == 1) {
        fprintf(stdout, "main:  about to cancel thread\n");
        s = pthread_cancel(thr);
        if (s != 0) {
            fprintf(stderr, "failed to call pthread_cancel(), %d\n", errno);
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stdout, "main:  about to signal condition variable\n");
        glob = 1;
        s = pthread_cond_signal(&cond);
        if (s != 0) {
            fprintf(stderr, "failed to call pthread_cond_signal(), %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }

    s = pthread_join(thr, &res);
    if (s != 0) {
        fprintf(stderr, "failed to call pthread_join(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if (res == PTHREAD_CANCELED) {
        fprintf(stdout, "main: thread was canceled\n");
    } else {
        fprintf(stdout, "main: thread terminated normally\n");
    }

    exit(EXIT_SUCCESS);
}