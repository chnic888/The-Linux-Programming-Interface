# LOGIN ACCOUNTING

- `Login accounting`是关注记录哪些用户当前登录系统以及记录过去的登录和登出的行为

## Overview of the utmp and wtmp Files

- `utmp`文件维护着当前正在登录系统的用户记录，每一个用户登录系统时都会向`utmp`文件写入一条对应的记录，当用户登出时候该记录会被删除。
	- `who(1)`命令会使用此文件来显示当前登录用户列表
	- Linux中，`utmp`文件位置在`/var/run/utmp`
	- 使用`_PATH_UTMP`来获取`utmp`文件路径，避免使用hard code
- `wtmp`文件所有用户登录和登出的audit log，每一个用户登录系统时写入`utmp`的记录会同时写入`wtmp`，用户登出时候还会增加和登入同样的记录，但是`ut_user`会被清零
	- `last(1)`命令会使用此文件来显示和过滤`wtmp`文件中的内容
	- Linux中，`utmp`文件位置在`/var/log/wtmp`
	- 使用`_PATH_WTMP`来获取`wtmp`文件路径，避免使用hard code

## The utmpx API

## The utmpx Structure

- `utmp`和`wtmp`文件由`utmpx`记录组成

```c
#define _GNU_SOURCE /* Without _GNU_SOURCE the two field names below are prepended by "__" */
struct exit_status {  
    short e_termination; /* Process termination status (signal) */
    short e_exit; /* Process exit status */
};

#define __UT_LINESIZE 32
#define __UT_NAMESIZE 32
#define __UT_HOSTSIZE 256

struct utmpx {
    short ut_type; /* Type of record */
    pid_t ut_pid; /* PID of login process */
    char ut_line[__UT_LINESIZE]; /* Terminal device name */
    char ut_id[4]; /* Suffix from terminal name, or ID field from inittab(5) */
    char ut_user[__UT_NAMESIZE]; /* Username */
    char ut_host[__UT_HOSTSIZE]; /* Hostname for remote login, or kernel version for run-level messages */
    struct exit_status ut_exit; /* Exit status of process marked as DEAD_PROCESS (not filled in by init(8) on Linux) */
    long ut_session; /* Session ID */
    struct timeval ut_tv; /* Time when entry was made */
    int32_t ut_addr_v6[4]; /* IP address of remote host (IPv4 address uses just ut_addr_v6[0], with other elements set to 0) */
    char __unused[20]; /* Reserved for future use */
};
```

- `utmpx` struct中的string类型的字段除非数组完全被填满，否则均是以null结尾

## Retrieving Information from the utmp and wtmp Files

- `utmpx`函数都使用了`current location`的概念，即从文件中的`current location`来读取记录，并且每个函数都会更新这个location

```c
#include <utmpx.h>

void setutxent(void);
```

- `setutxent()` 会将`utmp`文件的`current location`重新设置为起始

```c
#include <utmpx.h>

void endutxent(void);
```

- `endutxent()`会关闭已经打开的`utmp`文件，`utmp`文件可以被`setutxent()`和`getutx*()`所打开

```c
#include <utmpx.h>

struct utmpx *getutxent(void);
struct utmpx *getutxid(const struct utmpx *ut);
struct utmpx *getutxline(const struct utmpx *ut);
```

- `getutxent()` `getutxid()` `getutxline()`会从`utmp`文件内读出一条记录并且返回一个指向改记录的`utmpx`struct的指针

```c
#define _GNU_SOURCE
#include <utmpx.h>

int utmpxname(const char *file);
```

- 默认情况下，`getutx*()`函数都会使用`utmp`文件，如果使用另外一个文件，比如`wtmp`文件，应当首先调用`utmpxname()`，并指定相应的路径

## Retrieving the Login Name: getlogin()

```c
#include <unistd.h>

char *getlogin(void);
```

- `getlogin()`返回登录到calling process的controlling terminal的用户名，会使用`utmp`文件内维护的信息

## Updating the utmp and wtmp Files for a Login Session

```c
#include <utmpx.h>

struct utmpx *pututxline(const struct utmpx *ut);
```

- `pututxline()`将`ut`指向的struct `utmpx`写入到`/var/run/utmp`中，或者是之前调用`utmpxname()`时所指定的另外一文件

```c
#define _GNU_SOURCE
#include <utmpx.h>

void updwtmpx(char *wtmpx_file, struct utmpx *ut);
```

- `updwtmpx()`将`ut`指向的struct `utmpx`附加到`wtmpx_file`所指定的文件尾部

## The lastlog

- `lastlog`文件记录着每个用户最近一次登录到系统的时间
	- `login`程序可以通过`lastlog`文件来通知用户上次登录的时间
	- Linux中，`lastlog`文件位置在`/var/log/lastlog`
	- 使用`__PATH_LASTLOG`来获取`lastlog`文件路径，避免使用hard code

```c
#define UT_NAMESIZE 32
#define UT_HOSTSIZE 256

struct lastlog {
    time_t ll_time; /* Time of last login */q
    char ll_line[UT_NAMESIZE]; /* Terminal for remote login */
    char ll_host[UT_HOSTSIZE]; /* Hostname for remote login */
};
```

- `lastlog` struct是不包含用户名和uid的，`lastlog`
  文件使用uid作为记录的索引，因此如果需要找到uid为1000的用户的记录，记录在文件中的位置为`(1000 * sizeof(struct lastlog))`