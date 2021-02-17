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

```c
#include <sys/time.h>

int getitimer(int which, struct itimerval *curr_value);
```
## Scheduling and Accuracy of Timers

## Setting Timeouts on Blocking Operations

## Suspending Execution for a Fixed Interval (Sleeping)

## POSIX Clocks

## POSIX Interval Timers

## Timers That Notify via File Descriptors: the timerfd API