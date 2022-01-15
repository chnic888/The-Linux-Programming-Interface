# DIRECTORIES AND LINKS

## Directories and (Hard) Links
- 硬连接的所有的文件均指向相同的i-node条目，硬连接的数据记录在i-node条目的连接计数器中
- `rm`命令从目录列表中删除文件名，并把i-node条目中的硬连接数量减1，如果计数器降为0，还会释放i-node和所引用的数据块

![18-1.png](img/18-1.png)

## Symbolic (Soft) Links
- i-node条目中的连接计数器并不计入符号连接的数量

![18-2.png](img/18-2.png)

## Creating and Removing (Hard) Links: link() and unlink()
- kernel不仅要为每个i-node维护link计数，还需要为每个文件维护open file descriptions的计数
- 当指向文件的最后一个link被移除时，如果仍有process持有这个文件的fd，在所有fd关闭之后，文件才会被真正的删除

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
- 默认情况下，`nftw()`会针对给定的树执行未排序的前序遍历，即优先处理各目录，然后再处理目录中的文件和子目录
- 每次`func`调用都会返回一个整数值，如果返回0则`nftw`会继续遍历，直至遍历完成并返回0给调用者。如果调用`func`返回非0，`nftw`会立刻停止对树的遍历，并且返回相同的非0值

```c
#define _XOPEN_SOURCE 500
#include <ftw.h>

int nftw(const char *dirpath, 
         int (*func) (const char *pathname, 
                      const struct stat *statbuf, 
                      int typeflag, 
                      struct FTW *ftwbuf), 
         int nopenfd, 
         int flags);
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
- 解引所有符号链接，包括`/.`和`/..`
- 生成一个绝对路径的字符串且以空字符串结尾
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