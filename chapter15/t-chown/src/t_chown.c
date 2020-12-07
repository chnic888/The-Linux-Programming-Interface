#include "t_chown.h"
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

static gid_t groupIdFromName(const char *);

static uid_t userIdFromName(const char *);

int tChown(int argc, char *argv[]) {
    uid_t uid;
    gid_t gid;
    int j;
    bool errFnd;

    if (argc < 3 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "%s owner group [file...]\n"
                        "       owner or group can be '-', "
                        "meaning leave unchanged\n", argv[0]);
        return (EXIT_FAILURE);
    }

    if (strcmp(argv[1], "-") == 0) {
        uid = -1;
    } else {
        uid = userIdFromName(argv[1]);
        if (uid == -1) {
            fprintf(stderr, "No such usr (%s)\n", argv[1]);
            return (EXIT_FAILURE);
        }
    }

    if (strcmp(argv[2], "-") == 0) {
        gid = -1;
    } else {
        gid = groupIdFromName(argv[2]);
        if (gid == -1) {
            fprintf(stderr, "No such group (%s)\n", argv[2]);
            return (EXIT_FAILURE);
        }
    }

    errFnd = false;
    for (j = 3; j < argc; j++) {
        if (chown(argv[j], uid, gid) == -1) {
            fprintf(stderr, "failed to chown %s. %d\n", argv[j], errno);
            errFnd = true;
        }
    }

    return (errFnd ? EXIT_FAILURE : EXIT_SUCCESS);
}

static gid_t groupIdFromName(const char *name) {
    struct group *grp = getgrnam(name);
    if (grp == NULL) {
        return -1;
    }

    return grp->gr_gid;
}

static uid_t userIdFromName(const char *name) {
    struct passwd *pwd = getpwnam(name);
    if (pwd == NULL) {
        return -1;
    }
    return pwd->pw_uid;
}