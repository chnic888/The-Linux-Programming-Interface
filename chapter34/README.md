# PROCESS GROUPS, SESSIONS, AND JOB CONTROL

## Overview
- `process group` 由一个或多个共享同一个`process group identifier(PGID)`的process组成
- `PGID`和`PID`的类型都是`pid_t`，每一个`process group`都会有一个leader，也就是创建该`process group`的process，leader的`PID`会成为`process group`的`PGID`，新process会继承parent process的`PGID`
- `process group lifetime` 起始于leader process创建group，终止于最后一个process退出group，且leader process无需是group中最后的process
- `session` 是一组`process group`的集合，process是哪个session的成员是由`session identifier(SID)`来决定的，且`SID`和`PID`的类型都是`pid_t`，第一个创建`session`的process为`session leader`，leader的`PID`会成为`session`的`SID`，新process会继承parent process的`SID`
- `session`中的所有process共享一个单独的`controlling terminal`
- 在任何时候，`session`中其中一个`process group`会成为`foreground process group`，其他的groups则会成为`background process groups`，且只有`foreground process group`才能从`controlling terminal`中读取输入
- 当用户在`controlling terminal`输入一个会产生字符的signal时，该signal会被发送给`foreground process group`的所有成员
- 当和`controlling terminal`建立连接之后，session leader则会成为该terminal的`controlling process`，当断开与terminal连接时，kernel会向session leader发送一个`SIGHUP`类型的signal
- 对于交互式登录，用户需要通过`controlling terminal`来进行登录，而登录shell是`session leader`和`controlling process`，对于窗口化环境中，`controlling terminal`是一个伪终端，每一个`terminal window`都会有一个独立的`session`

```shell
#Display the PID of the shell
$ echo $$ 
400

#Creates 2 processes in background group
$ find / 2> /dev/null | wc -l & 
[1] 659

#Creates 2 processes in foreground group
$ sort < longlist | uniq -c 
```

![34-1.png](./img/34-1.png)

## Process Groups
```c
#include <unistd.h>

pid_t getpgrp(void);
```
-`getpgrp()` 如果返回值和calling process的PID相等的话，则说明calling process为process group中的leader

```c
#include <unistd.h>

int setpgid(pid_t pid, pid_t pgid);
```
- `setpgid()`由`pid`指定的process的PGID被设置成为`pgid`
- 如果`pid`为0，则calling process的PGID会被设置成为`pgid`
- 如果`pgid`为0，由`pid`指定的process的PGID被设置成为`pid`
- 如果`pid`和`pgid`指向同一process，则会创建一个新的`process group`且该process会成为leader
- 如果`pid`和`pgid`的值不同，则会将`pid`指定的process移动到`pgid`所指定的`process group`中

## Sessions
```c
#define _XOPEN_SOURCE 500
#include <unistd.h>

pid_t getsid(pid_t pid);
```

```c
#include <unistd.h>

pid_t setsid(void);
```
## Controlling Terminals and Controlling Processes
```c
#include <stdio.h> /* Defines L_ctermid constant */

char *ctermid(char *ttyname);
```

## Foreground and Background Process Groups
```c
#include <unistd.h>

pid_t tcgetpgrp(int fd);
int tcsetpgrp(int fd, pid_t pgid);
```

## The SIGHUP Signal

### Handling of SIGHUP by the Shell

### SIGHUP and Termination of the Controlling Process

## Job Control

### Using Job Control Within the Shell

### Implementing Job Control

### Handling Job-Control Signals

### Orphaned Process Groups (and SIGHUP Revisited)