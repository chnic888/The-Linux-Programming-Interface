# TIME

## Calendar Time
```c
#include <sys/time.h>

int gettimeofday(struct timeval *tv, struct timezone *tz);

struct timeval {
    time_t tv_sec; /* Seconds since 00:00:00, 1 Jan 1970 UTC */
    suseconds_t tv_usec; /* Additional microseconds (long int) */
};
```
```c
#include <time.h>

time_t time(time_t *timep);
```

## Time-Conversion Functions
![10-1.png](img/10-1.png)