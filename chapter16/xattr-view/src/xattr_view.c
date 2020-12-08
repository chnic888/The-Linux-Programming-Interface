#include "xattr_view.h"
#include <sys/xattr.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define XATTR_SIZE 10000

static void usageError(char *);

int xattrView(int argc, char *argv[]) {
    char list[XATTR_SIZE], value[XATTR_SIZE];
    ssize_t listLen, valueLen;
    int ns, j, k, opt;
    bool hexDisplay;

    hexDisplay = false;

    while ((opt = getopt(argc, argv, "x")) != -1) {
        switch (opt) {
            case 'x':
                hexDisplay = true;
                break;
            case '?':
                usageError(argv[0]);
            default:
                exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc + 2) {
        usageError(argv[0]);
    }

    for (j = optind; j < argc; j++) {
        listLen = listxattr(argv[j], list, XATTR_SIZE);
        if (listLen == -1) {
            fprintf(stderr, "failed to call listxattr(), %d\n", errno);
            return (EXIT_FAILURE);
        }

        fprintf(stdout, "%s:\n", argv[j]);
        for (ns = 0; ns < listLen; ns += strlen(&list[ns]) + 1) {
            fprintf(stdout, "   name=%s; ", &list[ns]);

            valueLen = getxattr(argv[j], &list[ns], value, XATTR_SIZE);
            if (valueLen == -1) {
                fprintf(stderr, "cloud not get value");
            } else if (!hexDisplay) {
                fprintf(stdout, "value=%.*s", (int) valueLen, value);
            } else {
                fprintf(stderr, "value=");
                for (k = 0; k < valueLen; k++) {
                    fprintf(stdout, "%02x ", (unsigned int) value[k]);
                }
            }

            fprintf(stdout, "\n");
        }

        fprintf(stdout, "\n");
    }
    
    return (EXIT_SUCCESS);
}

static void usageError(char *progName) {
    fprintf(stderr, "Usage: %s [-x] file...\n", progName);
    exit(EXIT_FAILURE);
}