#include "getpwnam.h"

void printPWInfo(struct passwd *pwUser) {
    fprintf(stdout, "username: %s\n", pwUser->pw_name);
    fprintf(stdout, "encrypted password %s\n", pwUser->pw_passwd);
    fprintf(stdout, "user id: %d\n", pwUser->pw_uid);
    fprintf(stdout, "group id: %d\n", pwUser->pw_gid);
    fprintf(stdout, "comment: %s\n", pwUser->pw_gecos);
    fprintf(stdout, "home dir: %s\n", pwUser->pw_dir);
    fprintf(stdout, "login shell: %s\n", pwUser->pw_shell);
}

int getpwnam_t(int argc, char *argv[]) {
    struct passwd *pwUser1 = getpwnam("avr");
    if (pwUser1 != NULL) {
        printPWInfo(pwUser1);
    }

    struct passwd *pwUser2 = getpwnam("tsr");
    if (pwUser2 != NULL) {
        printPWInfo(pwUser2);
    }

    return (EXIT_SUCCESS);
}