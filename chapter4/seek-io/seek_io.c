#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "seek_io.h"

long getOffset(char *);

int seekio(int argc, char *argv[]) {
    size_t len;
    long offset;
    int fd, ap, j;
    char *buf = NULL;
    ssize_t numRead, numWritten;

    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s file {r<length>|R<length>|w<string>|s<offset>}...\n", argv[0]);
        return (EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if (fd == -1) {
        fprintf(stderr, "failed to create file %s", argv[0]);

        return (EXIT_FAILURE);
    }

    for (ap = 2; ap < argc; ap++) {
        switch (argv[ap][0]) {
            case 'r':
            case 'R':
                len = getOffset(argv[ap]);
                buf = malloc(len * sizeof(char));
                if (buf == NULL) {
                    fprintf(stderr, "failed malloc memory size %ld\n", len);
                    return (EXIT_FAILURE);
                }

                numRead = read(fd, buf, len);
                if (numRead == -1) {
                    fprintf(stderr, "failed to read\n");
                    return (EXIT_FAILURE);
                }

                if (numRead == 0) {
                    fprintf(stdout, "empty\n");
                } else {
                    fprintf(stdout, "str %s\n", buf);
                }

                free(buf);
                buf = NULL;
                break;

            case 'w':
                numWritten = write(fd, &argv[ap][1], strlen(&argv[ap][1]));
                if (numWritten == -1) {
                    fprintf(stderr, "failed to write string %s", &argv[ap][1]);
                    return (EXIT_FAILURE);
                }
                fprintf(stdout, "%s wrote %ld bytes\n", argv[ap], numWritten);
                break;

            case 's':
                offset = getOffset(argv[ap]);
                if (lseek(fd, offset, SEEK_SET) == -1) {
                    fprintf(stderr, "failed to seek len %ld", offset);
                    return (EXIT_FAILURE);
                }
                fprintf(stdout, "%s seek successfully\n", argv[ap]);
                break;

            default:
                fprintf(stderr, "Argument must start with [rRws]: %s\n", argv[ap]);
        }
    }

    return (EXIT_SUCCESS);
}

long getOffset(char *str) {
    char *end = str + strlen(str) - 1;
    return strtol(str + 1, &end, 0);
}
