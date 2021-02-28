# MONITORING CHILD PROCESSES

## Waiting on a Child Process

### The wait() System Call
```c
#include <sys/wait.h>

pid_t wait(int *status)
```
- 如果没有任何一个calling process的child processes终止，且这些child process之前也并未被等待过，那么`wait()`调用将被阻塞，假如之后某个child process终止，那么`wait()`则会立刻返回
- 如果`status`非空，child process终止后的返回值会通过`status`指向的整数型变量返回
- kernel会为parent process下的所有child process添加process的CPU时间和资源使用情况统计信息
- 将终止的child process的pid作为`wait()`结果返回
- `wait()`返回-1的一个原因是calling process已经无child process可等待，这时errno会被设置为`ECHILD`

### The waitpid() System Call
```c
#include <sys/wait.h>

pid_t waitpid(pid_t pid, int *status, int options);
```

### The Wait Status Value

### Process Termination from a Signal Handler

### The waitid() System Call
```c
#include <sys/wait.h>

int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
```

### The wait3() and wait4() System Calls
```c
#define _BSD_SOURCE /* Or #define _XOPEN_SOURCE 500 for wait3() */
#include <sys/resource.h>
#include <sys/wait.h>

pid_t wait3(int *status, int options, struct rusage *rusage);
pid_t wait4(pid_t pid, int *status, int options, struct rusage *rusage);
```

## Orphans and Zombies

## The SIGCHLD Signal

### Establishing a Handler for SIGCHLD

### Delivery of SIGCHLD for Stopped Children

### Ignoring Dead Child Processes
