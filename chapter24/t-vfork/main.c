#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int istack = 222;

    switch (vfork()) {
        case -1:
            fprintf(stderr, "failed to call vfork(), %d\n", errno);
            exit(EXIT_FAILURE);
        case 0:
            sleep(0);

            write(STDOUT_FILENO, "Child executing\n", 16);
            istack *= 3;
            _exit(EXIT_SUCCESS);
        default:
            write(STDOUT_FILENO, "Parent executing\n", 17);
            fprintf(stdout, "istack=%d\n", istack);
            exit(EXIT_SUCCESS);
    }
}