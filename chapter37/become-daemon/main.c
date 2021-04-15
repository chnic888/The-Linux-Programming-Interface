#include <stdlib.h>
#include "become_daemon.h"
#include <unistd.h>

int main(int argc, char *argv[]) {
    char *endPtr = NULL;
    becomeDaemon((int) strtol(argv[1], &endPtr, 10));
    sleep(60);
    return 0;
}