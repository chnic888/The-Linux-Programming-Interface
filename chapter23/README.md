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

```c
#include <unistd.h>

unsigned int alarm(unsigned int seconds);
```
## Scheduling and Accuracy of Timers

## Setting Timeouts on Blocking Operations

## Suspending Execution for a Fixed Interval (Sleeping)

## POSIX Clocks

## POSIX Interval Timers

## Timers That Notify via File Descriptors: the timerfd API