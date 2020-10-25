#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE

#include "check_password.h"
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <shadow.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

int checkPassword(int argc, char *argv[]) {
    char *username, *password, *encrypted, *p;
    struct passwd *pwd;
    struct spwd *spwd;

    size_t len;
    long lnmax = sysconf(_SC_LOGIN_NAME_MAX);
    if (lnmax == -1) {
        lnmax = 256;
    }

    username = malloc(lnmax);
    if (username == NULL) {
        fprintf(stderr, "failed to malloc for username\n");
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Username: ");
    fflush(stdout);
    if (fgets(username, (int) lnmax, stdin) == NULL) {
        fprintf(stderr, "failed to get username\n");
        return (EXIT_FAILURE);
    }

    len = strlen(username);
    if (username[len - 1] == '\n') {
        username[len - 1] = '\0';
    }

    pwd = getpwnam(username);
    if (pwd == NULL) {
        fprintf(stderr, "failed to get password record for %s\n", username);
        return (EXIT_FAILURE);
    }

    spwd = getspnam(username);
    if (spwd == NULL && errno == EACCES) {
        fprintf(stdout, "no permission to read shadow password file\n");
        return (EXIT_FAILURE);
    }

    if (spwd != NULL) {
        pwd->pw_passwd = spwd->sp_pwdp;
    }

    password = getpass("Password: ");

    encrypted = crypt(password, pwd->pw_passwd);
    for (p = password; *p != '\0';) {
        *p++ = '\0';
    }

    if (encrypted == NULL) {
        fprintf(stdout, "failed to encrypted password\n");
        return (EXIT_FAILURE);
    }

    bool authOK = strcmp(encrypted, pwd->pw_passwd) == 0;
    if (!authOK) {
        fprintf(stdout, "incorrect password\n");
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "successfully authenticated: UID=%ld\n", (long) pwd->pw_uid);
    return (EXIT_SUCCESS);
}