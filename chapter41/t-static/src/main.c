#include "hello.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    fprintf(stdout, "Welcome ");
    sayHello();
    return (EXIT_SUCCESS);
}