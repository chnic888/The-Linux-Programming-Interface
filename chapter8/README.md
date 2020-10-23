# USERS AND GROUPS

## The Password File: /etc/passwd
```shell script
mtk:x:1000:100:Michael Kerrisk:/home/mtk:/bin/bash
```
- Login name
- Encrypted password
- User ID (UID)
- Group ID (GID)
- Comment
- Home directory
- Login shell

## The Shadow Password File: /etc/shadow

## The Group File: /etc/group
```shell script
users:x:100:
jambit:x:106:claus,felli,frank,harti,markus,martin,mtk,paul
```
- Group name
- Encrypted password
- Group ID (GID)
- User list

## Retrieving User and Group Information
```c
#include <pwd.h>

struct passwd *getpwnam(const char *name);
struct passwd *getpwuid(uid_t uid);

struct passwd {
 char *pw_name; /* Login name (username) */
 char *pw_passwd; /* Encrypted password */
 uid_t pw_uid; /* User ID */
 gid_t pw_gid; /* Group ID */
 char *pw_gecos; /* Comment (user information) */
 char *pw_dir; /* Initial working (home) directory */
 char *pw_shell; /* Login shell */
};
```
