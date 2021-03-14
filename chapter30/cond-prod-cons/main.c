#include <pthread.h>
#include <stdio.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int avail = 10;

void *producer(void *arg) {
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

    fprintf(stdout, "signal condition variable\n");
    if (pthread_cond_signal(&cond) != 0) {
        return NULL;
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t pt1, pt2, pt3, pt4;

    pthread_create(&pt1, NULL, producer, "1");
    pthread_create(&pt2, NULL, producer, "2");
    pthread_create(&pt3, NULL, producer, "3");
    pthread_create(&pt4, NULL, producer, "4");

    while (1) {
        pthread_mutex_lock(&mtx);

        while (avail == 0) {
            fprintf(stdout, "[main] about to sleep\n");
            pthread_cond_wait(&cond, &mtx);
            fprintf(stdout, "[main] sleep done\n");
        }

        if (avail > 0) {
            fprintf(stdout, "[main] consumer: %d -> ", avail);
            avail--;
            fprintf(stdout, "%d\n", avail);
        }

        pthread_mutex_unlock(&mtx);
    }
}