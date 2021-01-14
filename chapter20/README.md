# SIGNALS: FUNDAMENTAL CONCEPTS

## Concepts and Overview
- 信号是事件发生时对进程的一种通知机制，有时也被成为软件中断，所谓中断就是会打断程序执行的正常流程，大多情况下，无法预测信号到达的精确时间
- 一个有合适权限的进程可以向另外一个进程发送信号，进程也可以向自身发送信号
- kernel也可以向进程发送信号，大多数进程收到的信号来源都是kernel
  - 硬件发生异常
  - 用户键入了能够产生信号的终端特殊字符(Control-C or Control-D)
  - 发生了软件事件
---
- 信号通过某些事件来产生。一旦信号被产生，会稍后被传递给进程，而进程也会做出一些动作来相应信号。在产生和到达期间，信号处于pending状态
- 信号到达之后，进程根据信号执行如下默认操作之一
  - `ignored` 忽略信号
  - `terminated` 终止（杀死）进程
  - `core dump file` 产生一个core dump文件
  - `stopped` 停止进程：暂停进程的执行
  - `resumed` 恢复一个之前被暂停进程的执行
- `disposition` 改变针对于特定信号的默认相应行为，程序可以为信号设置如下的`disposition`
  - `default action` 继续采取默认行为
  - `ignored` 忽略信号
  - `signal handler` 执行信号处理器signal handler
---  
- 通知kernel去调用一个`signal handler`的行为可以被称之为`installing`或者`establishing`一个signal handler
- `signal handler`被调用之后，我们可以成为知信号已经被`handled`或`caught`

## Signal Types and Default Actions
| Name | Signal number | Description | SUSv3 | Default |
| --- | --- | --- | --- | --- |
| SIGABRT | 6 | Abort process | ● | core |
| SIGALRM | 14 | Real-time timer expired | ● | term |
| SIGBUS | 7 (SAMP=10) | Memory access error | ● | core |
| SIGCHLD | 17 (SA=20, MP=18) | Child terminated or stopped | ● | ignore |
| SIGCONT | 18 (SA=19, M=25, P=26) | Continue if stopped | ● | cont |
| SIGEMT | undef (SAMP=7) | Hardware fault | | term |
| SIGFPE | 8 | Arithmetic exception | ● | core |
| SIGHUP | 1 | Hangup | ● | term |
| SIGILL | 4 | Illegal instruction | ● | core |
| SIGINT | 2 | Terminal interrupt | ● | term |
| SIGIO / SIGPOLL | 29 (SA=23, MP=22) | I/O possible | ● | term |
| SIGKILL | 9 | Sure kill | ● | term |
| SIGPIPE | 13 | Broken pipe | ● | term |
| SIGPROF | 27 (M=29, P=21) | Profiling timer expired | ● | term |
| SIGPWR | 30 (SA=29, MP=19) | Power about to fail | | term |
| SIGQUIT | 3 | Terminal quit | ● | core
| SIGSEGV | 11 | Invalid memory reference | ● | core
| SIGSTKFLT | 16 (SAM=undef, P=36) | Stack fault on coprocessor | | term |
| SIGSTOP | 19 (SA=17, M=23, P=24) | Sure stop | ● | stop |
| SIGSYS | 31 (SAMP=12) | Invalid system call | ● | core |
| SIGTERM | 15 | Terminate process | ● | term |
| SIGTRAP | 5 | Trace/breakpoint trap | ● | core |
| SIGTSTP | 20 (SA=18, M=24, P=25) | Terminal stop | ● | stop |
| SIGTTIN | 21 (M=26, P=27) | Terminal read from BG | ● | stop |
| SIGTTOU | 22 (M=27, P=28) | Terminal write from BG | ● | stop |
| SIGURG | 23 (SA=16, M=21, P=29) | Urgent data on socket | ● | ignore |
| SIGUSR1 | 10 (SA=30, MP=16) | User-defined signal 1 | ● | term |
| SIGUSR2 | 12 (SA=31, MP=17) | User-defined signal 2 | ● | term |
| SIGVTALRM | 26 (M=28, P=20) | Virtual timer expired | ● | term |
| SIGWINCH | 28 (M=20, P=23) | Terminal window size change | | ignore |
| SIGXCPU | 24 (M=30, P=33) | CPU time limit exceeded | ● | core |
| SIGXFSZ | 25 (M=31, P=34) | File size limit exceeded | ● | core |

## Changing Signal Dispositions: signal()
- `signal()`在不同的Unix间存在实现上的差异，所以如果考虑程序的可移植性则绝不能使用此函数，应当使用`sigaction()`
- `singal()`的返回值是上一个signal的`disposition`，返回的是一个函数指针

```c
#include <signal.h>

void ( *signal(int sig, void (*handler)(int)) ) (int);
```
```c
typedef void (*sighandler_t)(int);

sighandler_t signal(int sig, sighandler_t handler);
```

## Introduction to Signal Handlers
- 调用信号处理程序，可能会随时打断主程序流程。内核会代表process来调用handler，当handler返回时，主程序会在handler打断的位置恢复执行
- 当kernel在调用信号handler时，他会把信号的整数编号作为参数传递给handler方法。如果handler只捕获一种类型的信号，那这个参数几乎就无用；如果需要处理多种信号，那么可以通过这个参数来判断信号类型

![20-1.png](img/20-1.png)

## Sending Signals: kill()
```c
#include <signal.h>
 
int kill(pid_t pid, int sig);
```
- `pid > 0` 信号会被发给pid指定的进程，接收信号的进程取决于参数pid。
- `pid = 0` 信号会被发送给进程组(process group ID == process group id of calling process)内的所有进程，也包括调用进程本身，接收信号的进程取决于调用进程的process group id。
- `pid < -1` 信号会被发送给进程组(process group ID == ABS(pid))内的所有进程，也就是向一个指定的进程组内的所有进程发信号。
- `pid = -1` 信号会被发送给所有调用进程有权给其发送信号的进程，除去调用进程本身和init(pid=1)，如果`privileged process`是调用进程，除了调用进程和init(pid=1)的进程之外的所有进程将会收到信号，这种调用方式也被称之为广播信号
- `pid无法找到指定匹配的进程` kill()调用失败，并将errno置为ESRCH

> 一个进程需要适当的权限才能向另一个进程发送信号。
- `privileged (CAP_KILL) process` 可以向任何进程发送信号
- `和root用户和root组一起运行的init(pid=1) process` 只能接收他内置的single handler可以处理的信号，以防止init进程被意外杀死
- `unprivileged process`
  ![20-2.png](img/20-2.png)
- `SIGCONT signal` unprivileged process可以给同session下的所有进程发送这个信号

## Other Ways of Sending Signals: raise() and killpg()
```c
#include <signal.h>

int raise(int sig);
```
```c
#include <signal.h>

int killpg(pid_t pgrp, int sig);
```