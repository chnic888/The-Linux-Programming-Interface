#include "acl_view.h"
#include <acl/libacl.h>
#include <sys/acl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

static void usageError(char *);

int aclView(int argc, char *argv[]) {
    acl_t acl;
    acl_type_t type;
    acl_entry_t entry;
    acl_tag_t tag;
    uid_t *uidp;
    gid_t *gid_p;
    acl_permset_t permset;
    char *name;
    int entryId, permVal, opt;

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

    return (EXIT_SUCCESS);
}

static void usageError(char *progName) {
    fprintf(stderr, "Usage: %s [-d] filename\n", progName);
    exit(EXIT_FAILURE);
}