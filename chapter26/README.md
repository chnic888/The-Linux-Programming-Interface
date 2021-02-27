# MONITORING CHILD PROCESSES

## Waiting on a Child Process

### The wait() System Call
```c
#include <sys/wait.h>

pid_t wait(int *status)
```

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
