# PROGRAM EXECUTION

## Executing a New Program: execve()
```c
#include <unistd.h>

int execve(const char *pathname, char *const argv[], char *const envp[]);
```
- `execve()`system call可以将新程序加载到process的的内存，旧的程序则会被丢弃，process原有的`stack` `data`和`heap`也会被新程序替换
- `pathname`是加载到process内存内新的program的路径名，既可以是绝对路径也可以是相对于calling process工作目录的相对路径
- `argv`指定了传递给新program命令行参数，并作为新程序C语言main函数的第二个`char *argv[]`参数
- `envp`指定了新program的环境列表，指向的字符串格式为`name=value`
- 由于`execve()`会加载新的program，因此一旦成功调用则永远不会有返回值，失败则会返回-1并可通过`errno`来判断错误原因

## The exec() Library Functions
```c
#include <unistd.h>

int execle(const char *pathname, const char *arg, ... /* , (char *) NULL, char *const envp[] */);
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