# EXTENDED ATTRIBUTES

## EA namespaces
- EA的命名格式为namespace.name，EA的命名格式为namespace来区分功能的大类，name在某一个命名空间内唯一标识一个EA
- namespace一共支持四个值user、trusted、system和security

## System Calls for Manipulating Extended Attributes

### Creating and modifying EAs
```c
#include <sys/xattr.h>

int setxattr(const char *pathname, const char *name, const void *value, size_t size, int flags);
int lsetxattr(const char *pathname, const char *name, const void *value, size_t size, int flags);
int fsetxattr(int fd, const char *name, const void *value, size_t size, int flags);
```

### Retrieving the value of an EA
```c
#include <sys/xattr.h>

ssize_t getxattr(const char *pathname, const char *name, void *value, size_t size);
ssize_t lgetxattr(const char *pathname, const char *name, void *value, size_t size);
ssize_t fgetxattr(int fd, const char *name, void *value, size_t size);
```

### Removing an EA
```c
#include <sys/xattr.h>

int removexattr(const char *pathname, const char *name);
int lremovexattr(const char *pathname, const char *name);
int fremovexattr(int fd, const char *name);v
```

### Retrieving the names of all EAs associated with a file
```c
#include <sys/xattr.h>

ssize_t listxattr(const char *pathname, char *list, size_t size);
ssize_t llistxattr(const char *pathname, char *list, size_t size);
ssize_t flistxattr(int fd, char *list, size_t size);
```