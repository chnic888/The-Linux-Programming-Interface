# SIGNALS: ADVANCED FEATURES

## Core Dump Files
```shell
#set the maximum size of core files created to unlimited
ulimit -c unlimited
```

### Naming the core dump file: /proc/sys/kernel/core_pattern
| Specifier | Replaced by |
| --- | --- |
| %c | Core file size soft resource limit (bytes; since Linux 2.6.24) |
| %e | Executable filename (without path prefix) |
| %g | Real group ID of dumped process |
| %h | Name of host system |
| %p | Process ID of dumped process |
| %s | Number of signal that terminated process |
| %t | Time of dump, in seconds since the Epoch |
| %u | Real user ID of dumped process |
| %% | A single % character |

## Special Cases for Delivery, Disposition, and Handling
- `SIGKILL` `SIGSTOP`信号总是执行默认的disposition，并且进程无法blocking这两种signals
- `SIGCONT`可以使得被`SIGSTOP` `SIGTSTP` `SIGTTIN` `SIGTTOU`停止的process恢复运行
- 一个处于停止状态下的process总是会被`SIGCONT`唤醒，无论process当下是否blocking或是ignoring了`SIGCONT`的信号
- `SIGCONT`和触发停止状态的signals是互斥的，也就是说process收到了`SIGCONT`signal之后会丢弃所有处于pending状态下会触发停止的signals，反之亦然。  
- 如果一个signal被传递给一个处于停止状态process，那么在process被`SIGCONT`恢复执行之前，signal实际上并未真正被传递给process，`SIGKILL`除外

## Interruptible and Uninterruptible Process Sleep States
- `TASK_INTERRUPTIBLE` aa
- `TASK_UNINTERRUPTIBLE` aa
- `TASK_KILLABLE` aa

## Hardware-Generated Signals
- `SIGBUS` `SIGFPE` `SIGILL` `SIGSEGV`通常是由硬件异常所产生，较少通过kill()的方式来发送此类的signals
- 在硬件异常的情况下，如果process从此类的signal handler返回，或是blocking或ignoring此类的信号，SUSv3没有定义process在此情况下应有的行为
- 正确处理硬件的产生的信号方法
    - 接受默认行为，终止进程
    - 要么实现一个不会正常返回的signal handler，要么通过_exit()终止进程，或者调用siglongjmp()跳出signal handler并把程序的控制权交到主程序内产生信号之外的某一个位置

## Synchronous and Asynchronous Signal Generation
signal可以被同步方式产生或异步方式产生。同步方式产生的signal会被立刻传递，异步方式产生的signal，在signal的产生和实际传递之间仍可能会有一个瞬时的延迟

- 硬件异常导致的signals`SIGBUS` `SIGFPE` `SIGILL` `SIGSEGV` `SIGEMT`
- process使用`raise()` `kill()` `killpg()`来给自身发送的signals

## Timing and Order of Signal Delivery

## Implementation and Portability of signal()

## Realtime Signals

## Waiting for a Signal Using a Mask: sigsuspend()

## Synchronously Waiting for a Signal

## Fetching Signals via a File Descriptor

## Interprocess Communication with Signals

## Earlier Signal APIs (System V and BSD)   