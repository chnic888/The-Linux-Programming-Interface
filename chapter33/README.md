# THREADS: FURTHER DETAILS

## Thread Stacks
- 当thread创建时，每个thread都有一个属于自己的`thread  stack`而且大小固定
- `pthread_attr_setstacksize()`设置的thread属性决定了`thread  stack`的大小
- `pthread_attr_setstack()`可以同时控制`thread  stack`的大小和位置，不过对`thread  stack`位置的设置会降低程序的可移植性
- `sysconf(_SC_THREAD_STACK_MIN)`设置可使用的最小`thread stack`

## Threads and Signals

### How the UNIX Signal Model Maps to Threads
- signal的action属于process维度，一个process内的任意thread收到了未经处理的`stop`和`terminate`类型的signal，那么将停止或者终止该process内的所有thread
- signal的disposition也属于process维度，process内的所有thread也共享同一个disposition，如果一个thread使用`sigaction()`来为某一signal创建disposition，那么当process收到此类信号时，process内的任何thread都会调用disposition，同理，如果一个thread对signal的disposition设置为`ignore`，那么所有thread都将忽略此类signal
- signal既可以指向process也可以指向特定的thread，
    - signal在是thread context下通过执行特殊的硬件指令产生的
    - thread试图对一个`broken pipe`进行写入操作时所产生的`SIGPIPE`类型的signal
    - 通过`pthread_kill()`或`pthread_sigqueue()`发送的signal
- 如果signal被发送给一个multithreaded process，且process也为此类的signal创建了handler，那么kernel会随意选择其中一个thread，并在thread执行对handler的调用
- signal mask属于单个thread维度，每个thread可以使用`pthread_sigmask()`来控制signal mask
- kernel会对process和每条thread的pending signal维护记录，thread内调用`sigpending()`会返回calling thread和process的pending signal的并集，新创建的thread的pending signal set会初始化为空
- 如果signal handler打断了`pthread_mutex_lock()`，`pthread_mutex_lock()`会自动重新开始，如果打断了`pthread_cond_wait()`，`pthread_cond_wait()`要么自动重新开始要么返回0`(spurious wake-up)`  
- alternate signal stack属于单个thread维度，新创建的thread并不从其创建者处获得`alternate signal stack` 

### Manipulating the Thread Signal Mask
```c
#include <signal.h>

int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset);
```
- 刚创建的新thread会从其创建者处继承`signal mask`的拷贝，thread可以使用`pthread_sigmask()`来获取当前`signal mask`或者修改`signal mask`

### Sending a Signal to a Thread
```c
#include <signal.h>

int pthread_kill(pthread_t thread, int sig);
```     
- `pthread_kill()`向同一process下的由参数`thread`指定的另一个thread发送由参数`sig`指定的signal
- thread ID仅能保证在同一process下的唯一性，因此无法使用`pthread_kill()`向其他process的thread发送signal

```c
#define _GNU_SOURCE
#include <signal.h>

int pthread_sigqueue(pthread_t thread, int sig, const union sigval value);
```
- `pthread_sigqueue()`为Linux下特有的函数，结合了`pthread_kill()`和`sigqueue()`，向同一个process中的另一thread发送携带数据的signal

### Dealing with Asynchronous Signals Sanely
-  Pthreads API的所有函数均不属于`async-signal-safe`函数，均无法在`signal handler`被安全调用
- 所有thread都阻塞process可能接收到的所有异步signals，最简单的做法是在创建thread之前，由main thread阻塞这些signals，则后续的每个thread都会继承main thread的signal mask的拷贝
- 创建一个来专门调用`sigwaitinfo()` `sigtimedwait()`或`sigwait()`的thread

```c
#include <signal.h>

int sigwait(const sigset_t *set, int *sig);
```
- `sigwait()`会等待`set`内的任意signal的到达，接收该signal并且在`sig`中将其返回

## Threads and Process Control

### Threads and exec()
- process内的任意thread调用了`exec()`则当前的`calling program`则会被完全替换，并且除了calling thread之外，其他所有的thread会立即消失。 
- thread的`destructors`和`cleanup handlers`也不会被执行，process的`mutexes`和`condition variables`也会消失，且之后TID也是不确定的

### Threads and fork()
- 当multithreaded process调用`fork()`，只有calling thread会被复制到child process中，并且其他thread都会在child process消失
- multithreaded process使用`fork()`推荐的做法是，在其后立刻调用`exec()`，因为`exec()`会导致所有Pthreads objects在child process中消失，因为新程序会覆盖原有内存
- 如果`fork()`之后无法调用`exec()`，Pthreads API提供了`fork handlers`机制来创建fork处理函数
```c
pthread_atfork(prepare_func, parent_func, child_func);
```
- 调用`fork()`所产生的child process，从调用`fork()`的thread处继承`fork handlers`

### Threads and exit()
- 如果任意thread执行了`exit()`或者main thread执行了return，那么所有thread均会消失，也不会执行`thread-specific data destructors`和`cleanup handlers`

## Thread Implementation Models

### Many-to-one (M:1) implementations (user-level threads)
- thread的创建、调度和同步`mutex locking` `waiting on condition variables`等全都通过一个user-space的threading library在process内处理
- kernel不会感知process内的thread的存在

### One-to-one (1:1) implementations (kernel-level threads)
- 每一个thread映射一个单独的KSE，kernel来负责thread的调度，并且thread同步操作也通过system calls在kernel中实现

### Many-to-many (M:N) implementations (two-level model)
- 每一个process可以和多个KSE相关，并且也可以把多个thread映射到一个KSE，允许kernel把一个应用上的不同threads映射到不同的CPU上

## Linux Implementations of POSIX Threads
- `LinuxThreads` 最初的Linux threading implementation，已过时
- `NPTL(Native POSIX Threads Library)` Linux threading implementation的现代版，所有新thread library都是基于NPTL来开发

### LinuxThreads
- 使用`clone()`来实现，flag为`CLONE_VM | CLONE_FILES | CLONE_FS | CLONE_SIGHAND`

### NPTL
- 增加`futex`作为同步机制

### Which Threading Implementation?

## Advanced Features of the Pthreads API
- `Realtime scheduling`
- `Process shared mutexes and condition variables`
- `Advanced thread-synchronization primitives`

