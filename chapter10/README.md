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

### Converting time_t to Printable Form
```c
#include <time.h>

//Wed Jun 8 14:22:34 2011
char *ctime(const time_t *timep);
```
- ctime()会自动把本地时区和DST加入到转换逻辑中
- ctime(), gmtime(), localtime(), asctime()函数共享同一返回字符数组和struct tm，也就是说调用任何一个函数都会修改这些共享值

### Converting Between time_t and Broken-Down Time
```c
#include <time.h>

struct tm *gmtime(const time_t *timep);

struct tm *localtime(const time_t *timep);

struct tm {
    int tm_sec; /* Seconds (0-60) */
    int tm_min; /* Minutes (0-59) */
    int tm_hour; /* Hours (0-23) */
    int tm_mday; /* Day of the month (1-31) */
    int tm_mon; /* Month (0-11) */
    int tm_year; /* Year since 1900 */
    int tm_wday; /* Day of the week (Sunday = 0)*/
    int tm_yday; /* Day in the year (0-365; 1 Jan = 0)*/
    int tm_isdst; /* Daylight saving time flag
                    > 0: DST is in effect;
                    = 0: DST is not effect;
                    < 0: DST information not available */
};
```
```c
#include <time.h>

time_t mktime(struct tm *timeptr);
```

### Converting Between Broken-Down Time and Printable Form
```c
#include <time.h>

char *asctime(const struct tm *timeptr);
```
- asctime()转换时，本地时区对转换没有影响，因为一个分解时间已经被localtime()或者gmtime()处理过