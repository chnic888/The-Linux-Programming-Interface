# PROCESS CREATION

## Overview of fork(), exit(), wait(), and execve()
- `fork()` 允许一个process(parent process)创建一个新的process(child process)。child process会复制parent process的`stack` `data` `heap`和`text segments`。可以将其看做是把parent process一分为二的，`fork()`方法也因此得名
- `exit(status)` 终止一个process，将process占用的所有资源(memory, open file descriptors等)归还给kernel以便后续的再分配。`status`表示process退出时的状态，parent process可以使用`wait()`来获取`status`的状态
- `wait(&status)` 如果child process尚未调用`exit(status)`，那么`wait()`会挂起parent process直到child process终止。child process终止时的状态通过`wait()`的`status`参数返回
- `execve(pathname, argv, envp)` 会加载一个新的程序(`程序路径名pathname`，`参数列表argv`， `环境变量列表envp`)到process的内存当中，现有的`program text`则会被丢弃，`stack` `data`和`heap`则会被刷新以便创建新程序，这一操作通常被称为执行一个新程序

![24-1.png](./img/24-1.png)

## Creating a New Process: fork()
```c
#include <unistd.h>

pid_t fork(void);
```

```c
pid_t childPid; /* Used in parent after successful fork() to record PID of child */

switch (childPid = fork()) {
    case -1: /* fork() failed */
        /* Handle error */
    case 0: /* Child of successful fork() comes here */
        /* Perform actions specific to child */
    default: /* Parent comes here after successful fork() */
        /* Perform actions specific to parent */
}
```
### File Sharing Between Parent and Child

### Memory Semantics of fork()

## The vfork() System Call

## Race Conditions After fork()

## Avoiding Race Conditions by Synchronizing with Signals