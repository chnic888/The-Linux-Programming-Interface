#include <pthread.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    pthread_mutex_t mtx;
    pthread_mutexattr_t mtxAttr;
    int s, type;

    s = pthread_mutexattr_init(&mtxAttr);
    if (s != 0) {
        exit(EXIT_FAILURE);
    }

    s = pthread_mutexattr_settype(&mtxAttr, PTHREAD_MUTEX_ERRORCHECK);
    if (s != 0) {
        exit(EXIT_FAILURE);
    }

    s = pthread_mutex_init(&mtx, &mtxAttr);
    if (s != 0) {
        exit(EXIT_FAILURE);
    }

    s = pthread_mutex_destroy(&mtx);
    if (s != 0) {
        exit(EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}