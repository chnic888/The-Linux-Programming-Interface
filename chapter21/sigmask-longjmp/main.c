#include "sigmask_longjmp.h"

int main(int argc, char *argv[]) {
    return sigmaskLongjmp(argc, argv);
}
