#include "ugid_functions.h"
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *userNameFromId(uid_t);

static uid_t userIdFromName(const char *);

static char *groupNameFromId(gid_t);

static gid_t groupIdFromName(const char *);

int getInt(const char *str) {
    char *endptr;
    return (int) strtol(str, &endptr, 10);
}

int ugidFunctions(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "%s uid/unm/gid/gnm arg", argv[0]);
        return (EXIT_FAILURE);
    }

    if (strcmp(argv[1], "uid") == 0) {
        char *userName = userNameFromId(getInt(argv[2]));
        if (userName == NULL) {
            fprintf(stdout, "no user found\n");
        } else {
            fprintf(stdout, "user name: %s\n", userName);
        }
        return (EXIT_SUCCESS);
    }

    if (strcmp(argv[1], "unm") == 0) {
        uid_t userId = userIdFromName(argv[2]);
        if (userId == -1) {
            fprintf(stdout, "no user found\n");
        } else {
            fprintf(stdout, "user id: %d\n", userId);
        }
        return (EXIT_SUCCESS);
    }

    if (strcmp(argv[1], "gid") == 0) {
        char *groupName = groupNameFromId(getInt(argv[2]));
        if (groupName == NULL) {
            fprintf(stdout, "no group found\n");
        } else {
            fprintf(stdout, "group name: %s\n", groupName);
        }
        return (EXIT_SUCCESS);
    }

    if (strcmp(argv[1], "gnm") == 0) {
        gid_t groupId = groupIdFromName(argv[2]);
        if (groupId == -1) {
            fprintf(stdout, "no group found\n");
        } else {
            fprintf(stdout, "group id: %d\n", groupId);
        }
        return (EXIT_SUCCESS);
    }

    fprintf(stderr, "unsupported command %s", argv[1]);
    return (EXIT_FAILURE);
}

static char *userNameFromId(uid_t uid) {
    struct passwd *pwd;
    pwd = getpwuid(uid);
    return (pwd == NULL) ? NULL : pwd->pw_name;
}

static uid_t userIdFromName(const char *name) {
    struct passwd *pwd;
    uid_t u;
    char *endptr;

    if (name == NULL || *name == '\0') {
        return -1;
    }

    u = strtol(name, &endptr, 10);
    if (*endptr == '\0') {
        return u;
    }

    pwd = getpwnam(name);
    if (pwd == NULL) {
        return -1;
    }

    return pwd->pw_uid;
}

static char *groupNameFromId(gid_t gid) {
    struct group *grp;

    grp = getgrgid(gid);
    return (grp == NULL) ? NULL : grp->gr_name;
}

static gid_t groupIdFromName(const char *name) {
    struct group *grp;
    gid_t g;
    char *endptr;

    if (name == NULL || *name == '\0') {
        return -1;
    }

    g = strtol(name, &endptr, 10);
    if (*endptr == '\0') {
        return g;
    }

    grp = getgrnam(name);
    if (grp == NULL) {
        return -1;
    }

    return grp->gr_gid;
}