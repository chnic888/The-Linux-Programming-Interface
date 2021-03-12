#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static int avail = 10;

static void *producer(void *arg) {
    char *id = (char *) arg;

    if (pthread_mutex_lock(&mtx) != 0) {
        return NULL;
    }

    fprintf(stdout, "[thread-%s] provider: %d -> ", id, avail);
    avail++;
    fprintf(stdout, "%d\n", avail);

    if (pthread_mutex_unlock(&mtx) != 0) {
        return NULL;
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t pt1, pt2, pt3;

    pthread_create(&pt1, NULL, producer, "1");
    pthread_create(&pt2, NULL, producer, "2");
    pthread_create(&pt3, NULL, producer, "3");

    for (;;) {
        pthread_mutex_lock(&mtx);

        if (avail > 0) {
            fprintf(stdout, "[main] consumer: %d -> ", avail);
            avail--;
            fprintf(stdout, "%d\n", avail);
        }

        pthread_mutex_unlock(&mtx);
    }
}