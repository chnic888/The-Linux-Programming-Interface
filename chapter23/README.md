# TIMERS AND SLEEPING

## Interval Timers
```c
#include <sys/time.h>
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);

struct itimerval {
    struct timeval it_interval; /* Interval for periodic timer */
    struct timeval it_value; /* Current value (time until next expiration) */
};

struct timeval {
    time_t tv_sec; /* Seconds */
    suseconds_t tv_usec; /* Microseconds (long int) */
};
```
- `ITIMER_REAL` 创建以真实时间倒计时的timer，timer到期之后会产生`SIGALRM`signal并发送给process
- `ITIMER_VIRTUAL` 创建以process虚拟时间倒计时的timer，timer到期之后会产生`SIGALRM`signal并发送给process
- `ITIMER_PROF` 创建`profiling`的timer，以process时间(user mode和kernel mode下时间的总和)的倒计时的timer，timer到期之后会产生`SIGPROF`signal并发送给process
- `SIGALRM`和`SIGPROF`的默认的disposition都是终止进程

```c
#include <sys/time.h>

int getitimer(int which, struct itimerval *curr_value);
```
- 可以在任何的时候调用`getitimer()`来获取当前的timer的状态，距离下次到期的剩余时间

```c
#include <unistd.h>

unsigned int alarm(unsigned int seconds);
```
- `seconds`表示timer到期的秒数，到期以后会向calling process发送`SIGALRM`signal
- 调用`alarm()`会覆盖之前的timer的设置，调用`alarm(0)`可以让现有的timer失效
    
## Scheduling and Accuracy of Timers
- 取决于系统加载和process的调度，process也许会在timer到期之后的一瞬间时间之后才会被调度，但是间隔性的时间不会受此影响

## Setting Timeouts on Blocking Operations
- `real-time timers`的用途之一是为某个阻塞的system call设定一个阻塞的时间上限

## Suspending Execution for a Fixed Interval (Sleeping)

### Low-Resolution Sleeping: sleep()
```c
#include <unistd.h>

unsigned int sleep(unsigned int seconds);
```
- 如果休眠正常结束，`sleep()`将返回0。如果被signal中断，`sleep()`将返回未休眠的秒数
- 考虑到可移植性，应该尽量避免将`sleep()`和`alarm()`以及`setitimer()`混用

### High-Resolution Sleeping: nanosleep()
```c
#define _POSIX_C_SOURCE 199309
#include <time.h>

int nanosleep(const struct timespec *request, struct timespec *remain);

struct timespec {
    time_t tv_sec; /* Seconds */
    long tv_nsec; /* Nanoseconds */
};
```

## POSIX Clocks

## POSIX Interval Timers

## Timers That Notify via File Descriptors: the timerfd API