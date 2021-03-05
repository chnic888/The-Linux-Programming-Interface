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

| Function | Specification of program file (–, p) | Specification of arguments (v, l) | Source of environment (e, –) |
| :---: | :---: | :---: | :---: |
| `execve()` | pathname |  array | envp argument |
| `execle()` | pathname | list | envp argument |
| `execlp()` | filename + PATH | list | caller’s environ |
| `execvp()` | filename + PATH |  array | caller’s environ |
| `execv()` | pathname |  array | caller’s environ |
| `execl()` | pathname | list | caller’s environ |

- `exec`后的单个字母代表着function提供的不同的功能
    - `p`代表是`filename + PATH`模式，否则则为`pathname`模式
    - `v`代表的是参数是以`array`的方式，`l`代表参数是以`list`的方式，两种方式都需要以`NULL`指针来结尾
    - `e`代表的是环境列表以`array`的方式显式的传递，否则则会复用calling process的环境列表

### The PATH Environment Variable
- child process会在被创建时从parent process处继承一份环境变量的副本
- 避免对于`set-user-ID`或`set-group-ID`的程序使用`execvp()`和`execlp()`，以防止执行恶意程序。实际操作中，应当使用已知安全的路径来替换之前定义好的`PATH`

### Specifying Program Arguments as a List
- 如果在使用`exec()`时已知参数的个数，可以使用`execle()` `execlp()` `execl()`来代替`exec()`

### Passing the Caller’s Environment to the New Program
- `execlp()` `execvp()` `execl()` `execv()`不允许显式的指定环境变量

### Executing a File Referred to by a Descriptor: fexecve()
```c
#define _GNU_SOURCE
#include <unistd.h>

int fexecve(int fd, char *const argv[], char *const envp[]);
```
- 可以使用`open()`打开并且验证文件内容之后再使用`fexecve()`执行，`open()`和`fexecve()`之间的时间内，文件有可能被替换，持有fd并不能阻止同名新文件被创建

## Interpreter Scripts
- `interpreter` 可以阅读文本命令并且执行这些命令的程序
- UNIX kernels运行interpreter scripts有两点要求
    - script文件必须具有可执行权限
    - script文件的起始行必须制定interpreter的路径名，并且有固定的格式
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
![27-2.png](./img/27-2.png)