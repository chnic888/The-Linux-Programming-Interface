# PROCESS CREATION

## Overview of fork(), exit(), wait(), and execve()
- `fork()`
- `exit(status)`
- `fork()`
- `fork()`

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