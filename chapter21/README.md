# SIGNALS: SIGNAL HANDLERS

## Designing Signal Handlers
signal handler应该设计的足够简单来避免race condition的风险
- signal handler设置一个全局变量并退出
- signal handler执行某种类型的清理工作，之后终止进程或使用nonlocal goto来讲stack解除

### Reentrant and Async-Signal-Safe Functions
- `reentrant` 多个线程可以simultaneously的安全的调用某一个函数，那这个函数就是可以被称为`reentrant`
- `reentrant` SUSv3定义的`reentrant`，一个函数在多线程为定义顺序的情况下被调用，甚至是交叉调用，调用效果依然是一致的，no side affect

### Standard async-signal-safe functions

#### Reentrant and nonreentrant functions
`async-signal-safe` 一个`async-signal-safe`函数指的是其函数实现可以被signal handler安全的调用。如果一个函数是`async-signal-safe`的则代表函数要么是`reentrant`要么不会被signal handler打断
- 确保signal handler的代码本身是`reentrant`而且只调用`async-signal-safe`的方法
- 当主程序执行`async-signal-unsafe`的方法或者操作一个有可能被signal handler更新的数据结构时，要阻塞信号的传递。阻塞信号有些时候比较困难，所以规则可以简化为signal handler中绝不调用`async-signal-unsafe`的函数

#### Use of errno inside signal handlers
```c
void handler(int sig) {
 int savedErrno;
 savedErrno = errno;
 /* Now we can execute a function that might modify errno */
 errno = savedErrno;
}
```

### Global Variables and the sig_atomic_t Data Type
所有主程序与signal handler之间共享全局变量都应该是如下的声明
```c
volatile sig_atomic_t flag;
```
- 当signal handler访问全局变量时，总是应该在声明变量时加上volatile关键字，从来防止编译器将其优化到寄存器当中
- `sig_atomic_t`类型的++和--在C语言当中并不能保证是atomic操作，因此使用`sig_atomic_t`时唯一能做的就是在signal handler里设置，在主程序里检查

## Other Methods of Terminating a Signal Handler
- 使用_exit()终止进程，在调用之前可以做一些cleanup的操作
- 使用kill()或者raise()来发送一个signal直接杀死进程
- 从signal handler中执行一个nonlocal goto
- 使用abort()来终止进程并且同时产生一个core dump

### Performing a Nonlocal Goto from a Signal Handler
```c
#include <setjmp.h>
int sigsetjmp(sigjmp_buf env, int savesigs);

void siglongjmp(sigjmp_buf env, int val);
```
- 一个signal handler被装载并被调用时，在调用之前kernel会把唤醒这个signal handler的signal以及act.sa_mask的signals放入process signal mask当中，并在signal handler正常返回的时把他们从signal mask中移除，因此普通的`longjmp()`在这里就存在问题
- 在调用`sigsetjmp()`时，如果savesigs为非0，process signal mask就会被存入env，然后使用带有相同env参数的siglongjmp()来恢复signal mask
- 如果savesigs为0，则不会保存和恢复process signal mask

### Terminating a Process Abnormally: abort()
```c
#include <stdlib.h>

void abort(void);
```
- `abort()`会发出SIGABRT的signal，除非signal handler捕获了SIGABRT的signal并且尚未返回，否则`abort()`总是会终止process
- SUSv3规定，无论是阻塞或是忽略SIGABRT的类型，`abort()`均不受影响

## Handling a Signal on an Alternate Stack: sigaltstack()
在kernel调用signal handler的时候，kernel会分配一个stack frame供signal handler来执行  
如果此时没有足够的空间来创建frame，会产生SIGSEGV信号，但是这个时候stack空间已经不足以为SIGSEGV分配一个signal handler的frame，此时SIGSEGV会执行SIG_DFL来terminated process  
为了确保对`SIGSEGV`的signal handler的调用，这个时候需要申请一块`alternate signal stack`的内存来保证对handler的调用  
```c
#include <signal.h>

int sigaltstack(const stack_t *sigstack, stack_t *old_sigstack);

typedef struct {
    void *ss_sp; /* Starting address of alternate stack */
    int ss_flags; /* Flags: SS_ONSTACK, SS_DISABLE */
    size_t ss_size; /* Size of alternate stack */
} stack_t;
```
- `sigstack`和`old_sigstack`两者之一均可为NULL，`sigstack`为NULL则返回当前`alternate signal stack`，`old_sigstack`为NULL则会试图创建新的`alternate signal stack`
- `SIGSTKSZ` ss_size的典型值，`MINSIGSTKSZ` ss_size的最小值  
- `SS_ONSTACK` 如果这个ss_flags被设置在old_sigstack时，则说明process当前正执行在`alternate signal stack`上。如果process已经执行在`alternate signal stack`上时，如果试图用`sigaltstack()`创建一个新的`alternate signal stack`则会收到EPERM错误
- `SS_DISABLE` 如果这个ss_flags由old_sigstack返回, 则说明当前没有`alternate signal stack`，如果为sigstack指定这个ss_flags,则会禁用一个当前已经被创建的`alternate signal stack`

## The SA_SIGINFO Flag


## Interruption and Restarting of System Calls