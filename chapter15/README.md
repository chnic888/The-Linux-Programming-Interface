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

## File Timestamps
- `st_atime`  文件的上次访问时间
- `st_mtime`  文件的上次修改时间
- `st_ctime`  文件状态上一次的变更时间、及对文件的i-node信息上一次的修改的时间

### Changing File Timestamps with utime() and utimes()
```c
#include <utime.h>

int utime(const char *pathname, const struct utimbuf *buf);
```

```c
#include <sys/time.h>

int utimes(const char *pathname, const struct timeval tv[2]);
```

```c
#include <sys/time.h>

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

## File Permissions

### Permissions on Regular Files
- `mode_t`

| Constant | Octal value | Permission bit |
| --- | --- | --- |
| S_ISUID | 04000 | Set-user-ID |
| S_ISGID | 02000 | Set-group-ID |
| S_ISVTX | 01000 |  Sticky |
| S_IRUSR | 0400 | User-read |
| S_IWUSR | 0200 | User-write |
| S_IXUSR | 0100 | User-execute |
| S_IRGRP | 040 | Group-read |
| S_IWGRP | 020 | Group-write |
| S_IXGRP | 010 | Group-execute |
| S_IROTH | 04 | Other-read |
| S_IWOTH | 02 | Other-write |
| S_IXOTH | 01 | Other-execute |

### Permissions on Directories
- `Read` 目录的内容可以被列出，比如ls命令
- `Write` 文件可以在目录内被创建、删除
- `Execute` 目录内的文件可以被访问

### Permission-Checking Algorithm
`内核使用的权限检查规则`
- 对于privileged进程，授于其所有的访问权限
- 如果进程的effective user ID和文件的owner user ID相同，文件owner所拥有的权限将会赋予进程
- 如果进程的effective group ID或者任何的supplementary group IDs与文件的group ID一致，文件的group所拥有的权限将会赋予进程
- 如果三种都不满足，文件的other所拥有的的权限将会被赋予进程

### Checking File Accessibility: access()
```c
#include <unistd.h>

int access(const char *pathname, int mode);
```
| Constant | Description |
| --- | --- |
| F_OK | Does the file exist? |
| R_OK | Can the file be read? |
| W_OK | Can the file be written? | 
| X_OK | Can the file be executed? |

### The Process File Mode Creation Mask: umask()
```c
#include <sys/stat.h>

mode_t umask(mode_t mask);
```
- 对于新建文件，kernel使用`open()`或`creat()`中的`mode`参数来指定权限
- 对于新建目录，kernel会根据`mkdir()`的`mode`参数来指定权限
- `umask`是process级别的属性，用来控制当process创建文件或者目录时那些权限为应该被**turn off**

### Changing File Permissions: chmod() and fchmod()
```c
#include <sys/stat.h>
int chmod(const char *pathname, mode_t mode);

#define _XOPEN_SOURCE 500 /* Or: #define _BSD_SOURCE */
#include <sys/stat.h>
int fchmod(int fd, mode_t mode);
```