#include "popen_glob.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "print_wait_status.h"

#define POPEN_FMT "/bin/ls -d %s 2> /dev/null"
#define PAT_SIZE 50
#define PCMD_BUF_SIZE (sizeof(POPEN_FMT) + PAT_SIZE)

int popenGlob(int argc, char *argv[]) {
    char pat[PAT_SIZE];
    char popenCMD[PCMD_BUF_SIZE];
    FILE *fp;
    bool badPattern;
    int status, fileCnt, j;
    size_t len;
    char pathname[PATH_MAX];

    for (;;) {
        fprintf(stdout, "pattern: ");
        fflush(stdout);
        if (fgets(pat, PAT_SIZE, stdin) == NULL) {
            break;
        }

        len = strlen(pat);
        if (len <= 1) {
            continue;
        }

        if (pat[len - 1] == '\n') {
            pat[len - 1] = '\0';
        }

        for (j = 0, badPattern = false; j < len && !badPattern; j++) {
            if (!isalnum((unsigned char) pat[j]) && strchr("_*?[^-].", pat[j]) == NULL) {
                badPattern = true;
            }
        }

        if (badPattern) {
            fprintf(stdout, "Bad pattern character: %c\n", pat[j - 1]);
            continue;
        }

        snprintf(popenCMD, PCMD_BUF_SIZE, POPEN_FMT, pat);
        popenCMD[PCMD_BUF_SIZE - 1] = '\0';

        fp = popen(popenCMD, "r");
        if (fp == NULL) {
            fprintf(stdout, "popen() failed\n");
            continue;
        }

        fileCnt = 0;
        while (fgets(pathname, PATH_MAX, fp) != NULL) {
            fprintf(stdout, "%s", pathname);
            fileCnt++;
        }

        status = pclose(fp);
        fprintf(stdout, "   %d matching file%s\n", fileCnt, (fileCnt != 1) ? "s" : "");
        fprintf(stdout, "   pclose() status == %#x\n", (unsigned int) status);
        if (status != -1) {
            printWaitStatus("\t", status);
        }
    }

    return EXIT_SUCCESS;
}