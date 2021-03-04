# PROGRAM EXECUTION

## Executing a New Program: execve()
```c
#include <unistd.h>

int execve(const char *pathname, char *const argv[], char *const envp[]);
```

## The exec() Library Functions
```c
#include <unistd.h>

int execle(const char *pathname, const char *arg, ... /* , (char *) NULL, char *const envp[] */ );
int execlp(const char *filename, const char *arg, ... /* , (char *) NULL */);
int execvp(const char *filename, char *const argv[]);
int execv(const char *pathname, char *const argv[]);
int execl(const char *pathname, const char *arg, ... /* , (char *) NULL */);
```

### The PATH Environment Variable

### Specifying Program Arguments as a List

### Passing the Caller’s Environment to the New Program

### Executing a File Referred to by a Descriptor: fexecve()
```c
#define _GNU_SOURCE
#include <unistd.h>

int fexecve(int fd, char *const argv[], char *const envp[]);
```

## Interpreter Scripts
```shell
#! interpreter-path [ optional-arg ]
```

### Execution of interpreter scripts
![27-1.png](./img/27-1.png)

## File Descriptors and exec()

### The close-on-exec flag (FD_CLOEXEC)

## Signals and exec()

## Executing a Shell Command: system()
```c
#include <stdlib.h>

int system(const char *command);
```

## Implementing system()