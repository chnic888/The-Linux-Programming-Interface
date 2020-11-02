#include "getpwnam_went.h"
#include <stdlib.h>
#include <stdio.h>
#include <pwd.h>
#include <string.h>

void printPWInfo(struct passwd *);

int getpwnamWent(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "%s name\n", argv[0]);
        return (EXIT_FAILURE);
    }

    struct passwd *pwd;
    while ((pwd = getpwent()) != NULL) {
        if (strcmp(pwd->pw_name, argv[1]) == 0) {
            break;
        }
    }

    printPWInfo(pwd);
    endpwent();
    
    return (EXIT_SUCCESS);
}

void printPWInfo(struct passwd *pwUser) {
    fprintf(stdout, "username: %s\n", pwUser->pw_name);
    fprintf(stdout, "encrypted password %s\n", pwUser->pw_passwd);
    fprintf(stdout, "user id: %d\n", pwUser->pw_uid);
    fprintf(stdout, "group id: %d\n", pwUser->pw_gid);
    fprintf(stdout, "comment: %s\n", pwUser->pw_gecos);
    fprintf(stdout, "home dir: %s\n", pwUser->pw_dir);
    fprintf(stdout, "login shell: %s\n", pwUser->pw_shell);
}