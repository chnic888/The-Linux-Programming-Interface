#define _GNU_SOURCE

#include "strerror.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_ERROR_LEN 256

static char buf[MAX_ERROR_LEN];

char *strerror(int err) {
//    if (err < 0 || err >= _sys_nerr ||)
    return NULL;
}