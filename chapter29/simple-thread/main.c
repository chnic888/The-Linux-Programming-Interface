#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

static void *threadFunc(void *arg) {
    char *s = (char *) arg;
    fprintf(stdout, "%s", s);
    return (void *) strlen(s);
}

int main(int argc, char *argv[]) {
    pthread_t t1;
    void *res;
    int s;

    s = pthread_create(&t1, NULL, threadFunc, "Hello World\n");
    if (s != 0) {
        fprintf(stderr, "failed to call pthread_create(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Message from main()\n");
    s = pthread_join(t1, &res);
    if (s != 0) {
        fprintf(stderr, "failed to call pthread_join(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Thread returned %ld\n", (long) res);
    exit(EXIT_SUCCESS);
}
