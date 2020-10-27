#define _GNU_SOURCE

#include "idshow.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fsuid.h>
#include <limits.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>

#define SG_SIZE (NGROUPS_MAX + 1)

static char *userNameFromId(uid_t);

static char *groupNameFromId(gid_t);

int idshow(int argc, char *argv[]) {
    uid_t ruid, euid, suid, fsuid;
    gid_t rgid, egid, sgid, fsgid;
    gid_t suppGroup[SG_SIZE];
    int numGroups, j;
    char *p;

    if (getresuid(&ruid, &euid, &suid) == -1) {
        fprintf(stderr, "failed to call getresuid(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    if (getresgid(&rgid, &egid, &sgid) == -1) {
        fprintf(stderr, "failed to call getresgid(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fsuid = setfsuid(0);
    fsgid = setfsgid(0);

    fprintf(stdout, "UID: ");
    p = userNameFromId(ruid);
    fprintf(stdout, "real=%s (%ld); ", (p == NULL) ? "???" : p, (long) ruid);

    p = userNameFromId(euid);
    fprintf(stdout, "effective=%s (%ld); ", (p == NULL) ? "???" : p, (long) euid);

    p = userNameFromId(suid);
    fprintf(stdout, "saved=%s (%ld); ", (p == NULL) ? "???" : p, (long) suid);

    p = userNameFromId(fsuid);
    fprintf(stdout, "fs=%s (%ld); ", (p == NULL) ? "???" : p, (long) fsuid);
    fprintf(stdout, "\n");

    fprintf(stdout, "GID: ");
    p = groupNameFromId(rgid);
    fprintf(stdout, "real=%s (%ld); ", (p == NULL) ? "???" : p, (long) rgid);

    p = groupNameFromId(egid);
    fprintf(stdout, "effective=%s (%ld); ", (p == NULL) ? "???" : p, (long) egid);

    p = groupNameFromId(sgid);
    fprintf(stdout, "saved=%s (%ld); ", (p == NULL) ? "???" : p, (long) sgid);

    p = groupNameFromId(fsgid);
    fprintf(stdout, "fs=%s (%ld); ", (p == NULL) ? "???" : p, (long) fsgid);
    fprintf(stdout, "\n");

    numGroups = getgroups(SG_SIZE, suppGroup);
    if (numGroups == -1) {
        fprintf(stderr, "failed to call getgroups(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    fprintf(stdout, "Supplementary groups (%d): ", numGroups);
    for (j = 0; j < numGroups; j++) {
        p = groupNameFromId(suppGroup[j]);
        fprintf(stdout, "%s (%ld) ", (p == NULL) ? "???" : p, (long) suppGroup[j]);
    }
    fprintf(stdout, "\n");

    return (EXIT_SUCCESS);
}

static char *userNameFromId(const uid_t uid) {
    struct passwd *pwd = getpwuid(uid);
    return (pwd == NULL) ? NULL : pwd->pw_name;
}

static char *groupNameFromId(const gid_t gid) {
    struct group *grp = getgrgid(gid);
    return (grp == NULL) ? NULL : grp->gr_name;
}