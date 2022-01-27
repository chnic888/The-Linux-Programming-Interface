# SYSTEM LIMITS AND OPTIONS

## Retrieving System Limits (and Options) at Run Time

```c
#include <unistd.h>

long sysconf(int name);
```

- sysconf的参数name的定义在<unistd.h>中，格式为 `_SC_*`
- 假如limit为未定时，sysconf会返回-1，errno不会被改变
- 假如调用出错，sysconf同样会返回-1，但是errno会被修改为错误值，因此在使用sysconf时必须初始化errno来区分indeterminate和error

## Retrieving File-Related Limits (and Options) at Run Time

```c
#include <unistd.h>

long pathconf(const char *pathname, int name);
long fpathconf(int fd, int name);
```

- `pathconf`和`fpathconf`的参数name定义在<unistd.h>中，格式为 `_PC_*`

## System Options

- 相应常量定义在<unistd.h>中，常量通常会被加一些前缀来标明他源于什么样的标准，比如`_POSIX_ or _XOPEN_`
- 值为-1，表示不支持该选项
- 值为0，表示可能支持这个选项，程序必须在运行时检查该选项是否被支持，可以调用sysconf()和pathconf()来检查
- 值大于0，表示支持这个选项