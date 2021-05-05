#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    void *libHandle;
    void (*funcp)(void);
    const char *err;

    if (argc != 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s lib-path func-name\n", argv[0]);
        return (EXIT_FAILURE);
    }

    libHandle = dlopen(argv[1], RTLD_LAZY);
    if (libHandle == NULL) {
        fprintf(stderr, "failed to open shared library %s, %d\n", argv[1], errno);
        exit(EXIT_FAILURE);
    }

    (void) dlerror();

    *((void **) &funcp) = dlsym(libHandle, argv[2]);
    err = dlerror();

    if (err != NULL) {
        fprintf(stderr, "failed to find the symbol %s, %s\n", argv[2], err);
        exit(EXIT_FAILURE);
    }

    if (funcp == NULL)
        fprintf(stdout, "%s is NULL\n", argv[2]);
    else
        (*funcp)();
    
    exit(EXIT_SUCCESS);
}