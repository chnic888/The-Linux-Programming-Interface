#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int opt;

    while ((opt = getopt(argc, argv, "m::r::")) != -1) {
        switch (opt) {
            case 'm':
                if (mkdir(argv[optind], S_IRUSR | S_IWUSR | S_IXUSR) == -1) {
                    fprintf(stderr, "failed to call mkdir(), %d\n", errno);
                    return (EXIT_FAILURE);
                }
                break;
            case 'r':
                if (rmdir(argv[optind]) == -1) {
                    fprintf(stderr, "failed to call rmdir(), %d\n", errno);
                    return (EXIT_FAILURE);
                }
                break;
            default:
                fprintf(stdout, "error");
                break;
        }
    }

    return (EXIT_SUCCESS);
}