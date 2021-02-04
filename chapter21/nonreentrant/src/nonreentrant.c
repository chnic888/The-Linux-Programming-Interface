#include "nonreentrant.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

static char *str2;
static int handled = 0;

int nonreentrant(int argc, char *argv[]) {
    return (EXIT_SUCCESS);
}