# DIRECTORIES AND LINKS

## Directories and (Hard) Links
- 硬连接的所有的文件均指向相同的i-node条目，硬连接的数据记录在i-node条目的连接计数器中
- `rm`命令从目录列表中删除文件名，并把i-node条目中的硬连接数量减1，如果计数器降为0，还会释放i-node和所引用的数据块

![18-1.png](img/18-1.png)

## Symbolic (Soft) Links
- i-node条目中的连接计数器并不计入符号连接的数量

![18-2.png](img/18-2.png)

## Creating and Removing (Hard) Links: link() and unlink()
```c
#include <unistd.h>

int link(const char *oldpath, const char *newpath);
```

```c
#include <unistd.h>

int unlink(const char *pathname);
```

## Changing the Name of a File: rename()
```c
#include <stdio.h>

int rename(const char *oldpath, const char *newpath);
```

## Working with Symbolic Links: symlink() and readlink()
```c
#include <unistd.h>

int symlink(const char *filepath, const char *linkpath);
```
```c
#include <unistd.h>

ssize_t readlink(const char *pathname, char *buffer, size_t bufsiz);
```

## Creating and Removing Directories: mkdir() and rmdir()
```c
#include <sys/stat.h>

int mkdir(const char *pathname, mode_t mode);
```
```c
#include <unistd.h>

int rmdir(const char *pathname);
```

## Removing a File or Directory: remove()
```c
#include <stdio.h>

int remove(const char *pathname);
```

## Reading Directories: opendir() and readdir()
```c
#include <dirent.h>

DIR *opendir(const char *dirpath);
```
```c
#include <dirent.h>

DIR *fdopendir(int fd);
```
```c
#include <dirent.h>

struct dirent *readdir(DIR *dirp);

struct dirent {
    ino_t d_ino; /* File i-node number */
    char d_name[]; /* Null-terminated name of file */
};
```
```c
#include <dirent.h>

void rewinddir(DIR *dirp);
```
```c
#include <dirent.h>

int closedir(DIR *dirp);
```
```c
#include <dirent.h>

int dirfd(DIR *dirp);
int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);
```

## File Tree Walking: nftw()
```c
#define _XOPEN_SOURCE 500
#include <ftw.h>

int nftw(const char *dirpath, int (*func) (const char *pathname, const struct stat *statbuf, int typeflag, struct FTW *ftwbuf), int nopenfd, int flags);
```

## The Current Working Directory of a Process
### Retrieving the current working directory
```c
#include <unistd.h>

char *getcwd(char *cwdbuf, size_t size);
```

### Changing the current working directory
```c
#include <unistd.h>

int chdir(const char *pathname);
```
```c
#define _XOPEN_SOURCE 500 /* Or: #define _BSD_SOURCE */
#include <unistd.h>

int fchdir(int fd);
```

## Operating Relative to a Directory File Descriptor

## Changing the Root Directory of a Process: chroot()
```c
#define _BSD_SOURCE
#include <unistd.h>

int chroot(const char *pathname);
```

## Resolving a Pathname: realpath()
```c
#include <stdlib.h>

char *realpath(const char *pathname, char *resolved_path);
```

## Parsing Pathname Strings: dirname() and basename()
```c
#include <libgen.h>

char *dirname(char *pathname);
char *basename(char *pathname);
```