# MONITORING FILE EVENTS

## Overview
- application使用`inotify_init()`创建`inotify`实例，system call返回一个file descriptor来引用`inotify`的实例
- application使用`inotify_add_watch()`来向`inotify`实例的watch list里添加item
    - watch item = pathname + bit mask，`bit mask`是对pathname的指定的监控集合
    - `inotify_add_watch()`会返回一个watch descriptor，供后续操作使用
- application调用`inotify`实例的`read()`方法来获取通知，每次`read()`会返回一个或多个`inotify_event`结构，结构中记录了被监控的pathname上发生的一个event
- application结束监控时会关闭`inotify`的file descriptor，并会自动移除`inotify`实例所有的watch items

## The inotify API
```c
#include <sys/inotify.h>

int inotify_init(void);
```

```c
#include <sys/inotify.h>

int inotify_add_watch(int fd, const char *pathname, uint32_t mask);
```
![19-1.png](img/19-1.png)

```c
#include <sys/inotify.h>

int inotify_rm_watch(int fd, uint32_t wd);
```

## inotify Events


## Reading inotify Events
```c
struct inotify_event {
    int wd; /* Watch descriptor on which event occurred */
    uint32_t mask; /* Bits describing event that occurred */
    uint32_t cookie; /* Cookie for related events (for rename()) */
    uint32_t len; /* Size of 'name' field */
    char name[]; /* Optional null-terminated filename */
};
```
![19-2.png](img/19-2.png)

## Queue Limits and /proc Files
