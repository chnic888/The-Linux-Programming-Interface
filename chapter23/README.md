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
- SUSv3规定`nanosleep()`不能用signal来实现，因此可以和`alarm()`或`setitimer()`混用
- 尽管`nanosleep()`没有使用signal来实现，但是依然可以被其他的signal handler打断

## POSIX Clocks

### Retrieving the Value of a Clock: clock_gettime()
```c
#define _POSIX_C_SOURCE 199309
#include <time.h>

int clock_gettime(clockid_t clockid, struct timespec *tp);
int clock_getres(clockid_t clockid, struct timespec *res);
```

| Clock ID |  Description |
| --- | --- |
| CLOCK_REALTIME | Settable system-wide real-time clock |
| CLOCK_MONOTONIC | Nonsettable monotonic clock |
| CLOCK_PROCESS_CPUTIME_ID | Per-process CPU-time clock (since Linux 2.6.12) |
| CLOCK_THREAD_CPUTIME_ID | Per-thread CPU-time clock (since Linux 2.6.12) |

### Setting the Value of a Clock: clock_settime()
```c
#define _POSIX_C_SOURCE 199309
#include <time.h>

int clock_settime(clockid_t clockid, const struct timespec *tp);
```

### Obtaining the Clock ID of a Specific Process or Thread
```c
#define _XOPEN_SOURCE 600
#include <time.h>

int clock_getcpuclockid(pid_t pid, clockid_t *clockid);
```

```c
#define _XOPEN_SOURCE 600
#include <pthread.h>
#include <time.h>

int pthread_getcpuclockid(pthread_t thread, clockid_t *clockid);
```

### Improved High-Resolution Sleeping: clock_nanosleep()
```c
#define _XOPEN_SOURCE 600
#include <time.h>

int clock_nanosleep(clockid_t clockid, int flags, const struct timespec *request, struct timespec *remain);
```

## POSIX Interval Timers

### Creating a Timer: timer_create()
```c
#define _POSIX_C_SOURCE 199309
#include <signal.h>
#include <time.h>

int timer_create(clockid_t clockid, struct sigevent *evp, timer_t *timerid);
```

### Arming and Disarming a Timer: timer_settime()
```c
#define _POSIX_C_SOURCE 199309
#include <time.h>

int timer_settime(timer_t timerid, int flags, const struct itimerspec *value, struct itimerspec *old_value);
```

### Retrieving the Current Value of a Timer: timer_gettime()
```c
#define _POSIX_C_SOURCE 199309
#include <time.h>

int timer_gettime(timer_t timerid, struct itimerspec *curr_value);
```

### Deleting a Timer: timer_delete()
```c
#define _POSIX_C_SOURCE 199309
#include <time.h>

int timer_delete(timer_t timerid);
```

### Notification via a Signal

### Timer Overruns
```c
#define _POSIX_C_SOURCE 199309
#include <time.h>

int timer_getoverrun(timer_t timerid);
```

### Notification via a Thread

## Timers That Notify via File Descriptors: the timerfd API
```c
#include <sys/timerfd.h>

int timerfd_create(int clockid, int flags);
```
- `timerfd_create()`创建一个新的timer对象，并且返回file descriptor来指向刚刚创建的对象
- 当对`timerfd_create()`创建的timer对象使用完毕之后，应当使用`close()`来关闭fd，以便kernel可以释放timer相关的资源

```c
#include <sys/timerfd.h>

int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
```
- `timerfd_settime()`可以启动或者停止fd所指向的timer

```c
#include <sys/timerfd.h>

int timerfd_gettime(int fd, struct itimerspec *curr_value);
```
- `timerfd_gettime()`返回fd所指向的timer的interval和remaining时间

### Interactions of timerfd with fork() and exec()
- 调用`fork()`期间，child process会继承`timerfd_create()`创建的fd的拷贝，fd和parent process的fd均指向同一timer，因此parent/child process都可以读取timer的expiration信息
- `timerfd_create()`创建的fd可以跨越`exec()`得以保存，已经配备的timer会在`exec()`之后继续生成expiration通知

### Reading from the timerfd file descriptor
- 一旦使用`timerfd_settime()`启动了timer，就可以通过`read()`从fd中去读取timer的expiration信息
- 从上一次使用`timerfd_settime()`修改设置或者上一次调用`read()`之后，如果一个或者多个的expiration发生，那么`read()`则会立刻返回，并在buffer中包含expiration数量