#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int foo (char i) {return i*2;}

int (*return_foo(int a, int b)) (char c){
    fprintf(stdout, "%d\n", a);
    fprintf(stdout, "%d\n", b);

    return foo;
}

int main(int argc, char *argv[]) {
    int (*a)(char) = (return_foo)(1, 2);
    fprintf(stdout, "%d\n", a(10));

    return (EXIT_SUCCESS);
}
