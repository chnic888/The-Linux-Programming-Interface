#include "acl_view.h"
#include <acl/libacl.h>
#include <sys/types.h>
#include <sys/acl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>

static void usageError(char *);

static char *groupNameFromId(gid_t);

static char *userNameFromId(uid_t);

int aclView(int argc, char *argv[]) {
    acl_t acl;
    acl_type_t type;
    acl_entry_t entry;
    acl_tag_t tag;
    uid_t *uidp;
    gid_t *gidp;
    acl_permset_t permset;
    char *name = NULL;
    int entryId, status, permVal, opt;

    type = ACL_TYPE_ACCESS;
    while ((opt = getopt(argc, argv, "d")) != -1) {
        switch (opt) {
            case 'd':
                type = ACL_TYPE_DEFAULT;
                break;
            case '?':
                usageError(argv[0]);
            default:
                usageError("invalid parameter\n");
        }
    }

    if (optind + 1 != argc) {
        usageError(argv[0]);
    }

    acl = acl_get_file(argv[optind], type);
    if (acl == NULL) {
        fprintf(stderr, "failed to call acl_get_file(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    for (entryId = ACL_FIRST_ENTRY;; entryId = ACL_NEXT_ENTRY) {
        status = acl_get_entry(acl, entryId, &entry);
        if (status == 0) {
            break;
        }

        if (status == -1) {
            fprintf(stderr, "failed to call acl_get_entry(), %d\n", errno);
            return (EXIT_FAILURE);
        }

        if (acl_get_tag_type(entry, &tag) == -1) {
            fprintf(stderr, "failed to call acl_get_tag_type(), %d\n", errno);
            return (EXIT_FAILURE);
        }

        fprintf(stdout, "%-12s", (tag == ACL_USER_OBJ) ? "usr_obj" :
                                 (tag == ACL_USER) ? "user" :
                                 (tag == ACL_GROUP_OBJ) ? "group_obj" :
                                 (tag == ACL_GROUP) ? "group" :
                                 (tag == ACL_MASK) ? "mask" :
                                 (tag == ACL_OTHER) ? "other" : "???");

        if (tag == ACL_USER) {
            uidp = acl_get_qualifier(entry);
            if (uidp == NULL) {
                fprintf(stderr, "failed to call acl_get_qualifier(), %d\n", errno);
                return (EXIT_FAILURE);
            }

            name = userNameFromId(*uidp);
            if (name == NULL) {
                fprintf(stdout, "%-8d", *uidp);
            } else {
                fprintf(stdout, "%-8s", name);
            }

            if (acl_free(uidp) == -1) {
                fprintf(stderr, "failed to call acl_free(), %d\n", errno);
                return (EXIT_FAILURE);
            }
        } else if (tag == ACL_GROUP) {
            gidp = acl_get_qualifier(entry);
            if (gidp == NULL) {
                fprintf(stderr, "failed to call acl_get_qualifier(), %d\n", errno);
                return (EXIT_FAILURE);
            }

            name = groupNameFromId(*gidp);
            if (name == NULL) {
                fprintf(stdout, "%-8d", *gidp);
            } else {
                fprintf(stdout, "%-8s", name);
            }

            if (acl_free(gidp) == -1) {
                fprintf(stderr, "failed to call acl_free(), %d\n", errno);
                return (EXIT_FAILURE);
            }
        } else {
            fprintf(stdout, "        ");
        }

        if (acl_get_permset(entry, &permset) == -1) {
            fprintf(stderr, "failed to call acl_get_permset(), %d\n", errno);
            return (EXIT_FAILURE);
        }

        permVal = acl_get_perm(permset, ACL_READ);
        if (permVal == -1) {
            fprintf(stderr, "failed to call acl_get_perm() ACL_READ, %d\n", errno);
            return (EXIT_FAILURE);
        }
        fprintf(stdout, "%c", (permVal == 1) ? 'r' : '-');

        permVal = acl_get_perm(permset, ACL_WRITE);
        if (permVal == -1) {
            fprintf(stderr, "failed to call acl_get_perm() ACL_WRITE, %d\n", errno);
            return (EXIT_FAILURE);
        }
        fprintf(stdout, "%c", (permVal == 1) ? 'w' : '-');

        permVal = acl_get_perm(permset, ACL_EXECUTE);
        if (permVal == -1) {
            fprintf(stderr, "failed to call acl_get_perm() ACL_EXECUTE, %d\n", errno);
            return (EXIT_FAILURE);
        }
        fprintf(stdout, "%c", (permVal == 1) ? 'x' : '-');

        fprintf(stdout, "\n");
    }

    return (EXIT_SUCCESS);
}

static char *userNameFromId(uid_t uid) {
    struct passwd *pwd;

    pwd = getpwuid(uid);
    return (pwd == NULL) ? NULL : pwd->pw_name;
}

static char *groupNameFromId(gid_t gid) {
    struct group *grp;

    grp = getgrgid(gid);
    return (grp == NULL) ? NULL : grp->gr_name;
}

static void usageError(char *progName) {
    fprintf(stderr, "Usage: %s [-d] filename\n", progName);
    exit(EXIT_FAILURE);
}