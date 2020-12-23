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
| Bit value | In |  Out | Description |
| --- | --- | --- | --- |
| IN_ACCESS | ● | ● | File was accessed (read()) |
| IN_ATTRIB | ● | ● | File metadata changed |
| IN_CLOSE_WRITE | ● | ● | File opened for writing was closed |
| IN_CLOSE_NOWRITE | ● | ● | File opened read-only was closed |
| IN_CREATE | ● | ● | File/directory created inside watched directory |
| IN_DELETE | ● | ● | File/directory deleted from within watched directory |
| IN_DELETE_SELF | ● | ● | Watched file/directory was itself deleted |
| IN_MODIFY | ● | ● | File was modified |
| IN_MOVE_SELF | ● | ● | Watched file/directory was itself moved |
| IN_MOVED_FROM | ● | ● | File moved out of watched directory |
| IN_MOVED_TO | ● | ● | File moved into watched directory |
| IN_OPEN | ● | ● | File was opened |
| IN_ALL_EVENTS | ● | | Shorthand for all of the above input events |
| IN_MOVE | ● | | Shorthand for IN_MOVED_FROM \| IN_MOVED_TO |
| IN_CLOSE | ● | | Shorthand for IN_CLOSE_WRITE \| IN_CLOSE_NOWRITE |
| IN_DONT_FOLLOW | ● | | Don’t dereference symbolic link (since Linux 2.6.15) |
| IN_MASK_ADD | ● | | Add events to current watch mask for pathname |
| IN_ONESHOT | ● | | Monitor pathname for just one event |
| IN_ONLYDIR | ● | | Fail if pathname is not a directory (since Linux 2.6.15) |

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