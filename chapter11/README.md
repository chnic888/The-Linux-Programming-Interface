# SYSTEM LIMITS AND OPTIONS

##  Retrieving System Limits (and Options) at Run Time
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