# FILE ATTRIBUTES

## Retrieving File Information: stat()
```c
#include <sys/stat.h>

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