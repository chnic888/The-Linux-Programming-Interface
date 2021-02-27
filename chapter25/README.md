# PROCESS TERMINATION

## Terminating a Process: _exit() and exit()
- `abnormal termination` 由signal的接收产生，并且signal的默认的disposition是终止当前process
- `normal termination` 使用`_exit()`system call终止当前process

```c
#include <unistd.h>

void _exit(int status);
```
- 虽然可以将`0-255`之间的任意值赋给`_exit()`的`status`参数并传递给parent process，如果指定的值大于128将在shell引发混乱

```c
#include <stdlib.h>

void exit(int status);
```
- 程序一般不会直接调用`_exit()`而是调用`exit()`
    - 调用通过`atexit()`和`on_exit()`注册的`exit handlers`，执行顺序与注册顺序相反
    - 刷新stdio流缓冲区，因为stdio的缓冲区是在process的`user-space memory`中维护的
    - 使用由status提供的值执行`_exit()`system call

## Details of Process Termination

## Exit Handlers
- `exit handler` 一个由开发者提供的函数，可在process生命周期内的任意时间点注册并在process调用`exit()`正常终止时自动执行。如果程序是异常终止则不会调用`exit handler`
- 避免使用`SIGKILL`来终止process，因为无法改变默认的disposition，建议使用`SIGTERM`来终止process

### Registering exit handlers
```c
#include <stdlib.h>

int atexit(void (*func)(void));
```
- 可以注册多个`exit handlers`，当应用程序调用`exit()`时，函数的执行顺序和注册顺序相反
- SUSv3要求kernel的实现允许一个process至少能够注册32个`exit handlers`，使用system call`sysconf(_SC_ATEXIT_MAX)`来定义可注册的`exit handlers`数量上限
- 通过`fork()`创建出的child process会继承parent process的所有的`exit handlers`，而调用`exec()`时则会移除已经注册的`exit handlers`

```c
#define _BSD_SOURCE /* Or: #define _SVID_SOURCE */
#include <stdlib.h>

int on_exit(void (*func)(int, void *), void *arg);
```
- 调用`on_exit()`时会传递给两个参数给func，第一个`int`是`exit()`的参数`status`，第二个是`arg`的参数拷贝
- `on_exit()`虽然比`atexit()`更灵活，但是如果有可移植性的需求，则应该避免使用`on_exit()`

## Interactions Between fork(), stdio Buffers, and _exit()
- asdad