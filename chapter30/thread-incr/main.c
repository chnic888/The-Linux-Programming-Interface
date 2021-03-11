#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

static int glob = 0;

static void *threadFunc(void *arg) {
    int loops = *((int *) arg);
    int loc, j;

    for (j = 0; j < loops; j++) {
        loc = glob;
        loc++;
        glob = loc;
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t t1, t2;
    int loops, s;
    char *endPtr = NULL;

    loops = (argc > 1) ? (int) strtol(argv[1], &endPtr, 10) : 10000000;
    endPtr = NULL;

    s = pthread_create(&t1, NULL, threadFunc, &loops);
    if (s != 0) {
        fprintf(stderr, "failed to call pthread_create(), %d\n", errno);
        return EXIT_FAILURE;
    }

    s = pthread_create(&t2, NULL, threadFunc, &loops);
    if (s != 0) {
        fprintf(stderr, "failed to call pthread_create(), %d\n", errno);
        return EXIT_FAILURE;
    }

    s = pthread_join(t1, NULL);
    if (s != 0) {
        fprintf(stderr, "failed to call pthread_join(), %d\n", errno);
        return EXIT_FAILURE;
    }

    s = pthread_join(t2, NULL);
    if (s != 0) {
        fprintf(stderr, "failed to call pthread_join(), %d\n", errno);
        return EXIT_FAILURE;
    }

    fprintf(stdout, "glob = %d\n", glob);
    exit(EXIT_SUCCESS);
}