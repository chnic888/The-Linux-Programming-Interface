# TIME
- `Real time`
  - 某个标准时间点，比如calendar时间
  - process生命周期内的某一个固定的时间点，通常为程序启动，也可以被叫做`elapsed`或`wall clock`
  
- `Process time` 一个process所使用的CPU时间总量

## Calendar Time
- calendar time存储为time_t类型的变量中

```c
#include <sys/time.h>

int gettimeofday(struct timeval *tv, struct timezone *tz);

struct timeval {
    time_t tv_sec; /* Seconds since 00:00:00, 1 Jan 1970 UTC */
    suseconds_t tv_usec; /* Additional microseconds (long int) */
};
```
- `gettimeofday()`返回calendar time存储在`tv`指向的缓冲区中

```c
#include <time.h>

time_t time(time_t *timep);
```
- 一般简单的采用`t = time(NULL)`方式来获取UTC时间即可

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

#### Converting from broken-down time to printable form
```c
#include <time.h>

char *asctime(const struct tm *timeptr);
```
- asctime()转换时，本地时区对转换没有影响，因为一个分解时间已经被localtime()或者gmtime()处理过

```c
#include <time.h>
size_t strftime(char *outstr, size_t maxsize, const char *format, const struct tm *timeptr);

#include "curr_time.h"
char *currTime(const char *format);
```

| Specifier | Description | Example |
| --- | --- | --- |
| %% | A % character | % |
| %a | Abbreviated weekday name | Tue |
| %A | Full weekday name | Tuesday |
| %b, %h | Abbreviated month name | Feb |
| %B | Full month name | February |
| %c | Date and time | Tue Feb 1 21:39:46 2011 |
| %d | Day of month (2 digits, 01 to 31) | 01 |
| %D | American date (same as %m/%d/%y) | 02/01/11 |
| %e | Day of month (2 characters) |  1 |
| %F | ISO date (same as %Y-%m-%d) | 2011-02-01 |
| %H | Hour (24-hour clock, 2 digits) | 21 |
| %I | Hour (12-hour clock, 2 digits) | 09 |
| %j | Day of year (3 digits, 001 to 366) | 032 |
| %m | Decimal month (2 digits, 01 to 12) | 02 |
| %M | Minute (2 digits) | 39 |
| %p | AM/PM | PM |
| %P | am/pm (GNU extension) | pm |
| %R | 24-hour time (same as %H:%M) | 21:39 |
| %S | Second (00 to 60) | 46 |
| %T | Time (same as %H:%M:%S) | 21:39:46 |
| %u | Weekday number (1 to 7, Monday = 1) | 2 |
| %U | Sunday week number (00 to 53) | 05 |
| %w | Weekday number (0 to 6, Sunday = 0) | 2 |
| %W | Monday week number (00 to 53) | 05 |
| %x | Date (localized) | 02/01/11 |
| %X | Time (localized) | 21:39:46 |
| %y | 2-digit year | 11 |
| %Y | 4-digit year | 2011 |
| %Z | Timezone name | CET |

#### Converting from printable form to broken-down time
```c
#define _XOPEN_SOURCE
#include <time.h>

char *strptime(const char *str, const char *format, struct tm *timeptr);
```
- 要保证这个函数的可移植性，要么str和format总所含输入信息足以设置tm的所有字段，要么在调用strptime()之前已经完成了适当的初始化，比如使用memset()函数

## Timezones
- Time Zone文件 /usr/share/zoneinfo
- LocalTime文件 /etc/localtime

## Locales
- Locales文件 /usr/share/locale 或 /usr/lib/locale
```
language[_territory[.codeset]][@modifier]
```
> language是双字母的ISO语言代码  
> territory是双字母的ISO国家代码  
> codeset表示字符编码集
> modifier提供了用于区区分前三个数据都一样的情况

```c
#include <locale.h>

char *setlocale(int category, const char *locale);
```
| Filename | Purpose |
| --- | --- |
| LC_CTYPE | A file containing character classifications (see isalpha(3)) and rules for case conversion |
| LC_COLLATE | A file containing the collation rules for a character set | 
| LC_MONETARY | A file containing formatting rules for monetary values (see localeconv(3) and <locale.h>) |
| LC_NUMERIC | A file containing formatting rules for numbers other than monetary values (see localeconv(3) and <locale.h>) |
| LC_TIME | A file containing formatting rules for dates and times |
| LC_MESSAGES | A directory containing files specifying formats and values used for affirmative and negative (yes/no) responses |

- `setlocale(LC_ALL, "")`地区被指定为空字符串的时，意味着从环境变量取得locale的设置

## Process Time
进程时间是进程创建后使用CPU的总量时间，可以被分为两部分

- `User CPU time` 是在user mode下所花费的时间数量，有时也被称为虚拟时间，它是程序访问CPU的时间
- `System CPU time` 是在kernel model下花费时间的数量，它是内核用户执行system call或者代表程序执行其他任务的时间

```c
#include <sys/times.h>

clock_t times(struct tms *buf);

struct tms {
    clock_t tms_utime; /* User CPU time used by caller */
    clock_t tms_stime; /* System CPU time used by caller */
    clock_t tms_cutime; /* User CPU time of all (waited for) children */
    clock_t tms_cstime; /* System CPU time of all (waited for) children */
};
```
- `clock_t`的时钟计时单元可以通过调用`sysconf(_SC_CLK_TCK)`来获得，然后用clock_t值来除以该数值来转换为秒

```c
#include <time.h>

clock_t clock(void);
```
- `clock()`作为一个简单的接口去获取process时间，返回一个calling process所使用的CPU总时间(user + system)
- `clock()`返回值的计量单位是`CLOCKS_PER_SEC`，用返回值除以该数值来获取process所使用CPU的秒数