#include "thread_multijoin.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

static pthread_cond_t threadDied = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;

static int totThreads = 0;
static int numLive = 0;

static int numUnjoined = 0;

enum tstate {
    TS_ALIVE,
    TS_TERMINATED,
    TS_JOINED
};

static struct {
    pthread_t tid;
    enum tstate state;
    int sleepTime;
} *thread;

void *threadFunc(void *arg) {
    int idx = *((int *) arg);
    int s;

    sleep(thread[idx].sleepTime);
    fprintf(stdout, "Thread %d terminating\n", idx);

    s = pthread_mutex_lock(&threadMutex);
    if (s != 0) {
        fprintf(stdout, "failed to call pthread_mutex_lock() thread, %d\n", errno);
        return NULL;
    }

    numUnjoined++;
    thread[idx].state = TS_TERMINATED;

    s = pthread_mutex_unlock(&threadMutex);
    if (s != 0) {
        fprintf(stdout, "failed to call pthread_mutex_unlock(), %d\n", errno);
        return NULL;
    }

    fprintf(stdout, "thread %d signal\n", idx);
    s = pthread_cond_signal(&threadDied);
    if (s != 0) {
        fprintf(stdout, "failed to call pthread_cond_signal(), %d\n", errno);
        return NULL;
    }

    fprintf(stdout, "thread[%d] free arg %p\n", idx, arg);
    free(arg);

    return NULL;
}

int threadMultijoin(int argc, char *argv[]) {
    int s, idx;
    char *endPtr = NULL;

    if (argc < 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stdout, "%s nsecs...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    thread = calloc(argc - 1, sizeof(*thread));
    if (thread == NULL) {
        fprintf(stdout, "failed to call calloc(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    for (idx = 0; idx < argc - 1; idx++) {
        thread[idx].sleepTime = (int) strtol(argv[idx + 1], &endPtr, 10);
        endPtr = NULL;
        thread[idx].state = TS_ALIVE;

        int *tmp = (int *) malloc(sizeof(int));
        *tmp = idx;

        fprintf(stdout, "thread[%d] pass arg %p\n", *tmp, tmp);
        s = pthread_create(&thread[idx].tid, NULL, threadFunc, tmp);
        if (s != 0) {
            fprintf(stdout, "failed to call pthread_create(), %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }

    totThreads = argc - 1;
    numLive = totThreads;

    while (numLive > 0) {
        s = pthread_mutex_lock(&threadMutex);
        if (s != 0) {
            fprintf(stdout, "failed to call pthread_mutex_lock(), %d\n", errno);
            exit(EXIT_FAILURE);
        }

        while (numUnjoined == 0) {
            s = pthread_cond_wait(&threadDied, &threadMutex);
            if (s != 0) {
                fprintf(stdout, "failed to call pthread_cond_wait(), %d\n", errno);
                exit(EXIT_FAILURE);
            }
        }

        for (idx = 0; idx < totThreads; idx++) {
            if (thread[idx].state == TS_TERMINATED) {
                s = pthread_join(thread[idx].tid, NULL);
                if (s != 0) {
                    fprintf(stdout, "failed to call pthread_join(), %d\n", errno);
                    exit(EXIT_FAILURE);
                }

                thread[idx].state = TS_JOINED;
                numLive--;
                numUnjoined--;

                fprintf(stdout, "Reaped thread %d (numLive=%d)\n", idx, numLive);
            }
        }

        s = pthread_mutex_unlock(&threadMutex);
        if (s != 0) {
            fprintf(stdout, "failed to call pthread_mutex_unlock(), %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}