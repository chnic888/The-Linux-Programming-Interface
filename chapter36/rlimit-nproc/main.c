#include "rlimit_nproc.h"

int main(int argc, char *argv[]) {
    return rlimitNproc(argc, argv);
}