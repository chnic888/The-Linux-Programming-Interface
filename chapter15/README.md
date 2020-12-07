# FILE ATTRIBUTES

## Retrieving File Information: stat()

```c
#include
<sys/stat.h>

int stat(const char *pathname, struct stat *statbuf);
int lstat(const char *pathname, struct stat *statbuf);
int fstat(int fd, struct stat *statbuf);
```

```c
struct stat {
dev_t st_dev; /* IDs of device on which file resides */
ino_t st_ino; /* I-node number of file */
mode_t st_mode; /* File type and permissions */
nlink_t st_nlink; /* Number of (hard) links to file */
uid_t st_uid; /* User ID of file owner */
gid_t st_gid; /* Group ID of file owner */
dev_t st_rdev; /* IDs for device special files */
off_t st_size; /* Total file size (bytes) */
blksize_t st_blksize; /* Optimal block size for I/O (bytes) */
blkcnt_t st_blocks; /* Number of (512B) blocks allocated */
time_t st_atime; /* Time of last file access */
time_t st_mtime; /* Time of last file modification */
time_t st_ctime; /* Time of last status change */
};
```

## File Timestamps

- `st_atime`  文件的上次访问时间
- `st_mtime`  文件的上次修改时间
- `st_ctime`  文件状态上一次的变更时间、及对文件的i-node信息上一次的修改的时间

### Changing File Timestamps with utime() and utimes()

```c
#include
<utime.h>

int utime(const char *pathname, const struct utimbuf *buf);
```

```c
#include
<sys/time.h>

int utimes(const char *pathname, const struct timeval tv[2]);
```

```c
#include
<sys/time.h>

int futimes(int fd, const struct timeval tv[2]);
int lutimes(const char *pathname, const struct timeval tv[2]);
```

## File Ownership

### Ownership of New Files

- 当文件被创建时，文件的user id来自进程的effective user ID。新文件的group id来自进程的effective group ID(System V默认行为)，或者父目录的group ID((the BSD
  behavior)

| File system mount option | Set-group-ID bit enabled on parent directory? | Group ownership of new file taken from |
| --- | --- | --- |
| –o grpid, –o bsdgroups | (ignored) | parent directory group ID |
| –o nogrpid, –o sysvgroups (default) | no | process effective group ID |
| –o nogrpid, –o sysvgroups (default) | yes | parent directory group ID |

### Changing File Ownership: chown(), fchown(), and lchown()
  
```c
#include <unistd.h>
int chown(const char *pathname, uid_t owner, gid_t group);

#define _XOPEN_SOURCE 500 /* Or: #define _BSD_SOURCE */
#include <unistd.h>
int lchown(const char *pathname, uid_t owner, gid_t group);
int fchown(int fd, uid_t owner, gid_t group);
```