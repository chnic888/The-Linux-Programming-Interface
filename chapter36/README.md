# PROCESS RESOURCES

## Process Resource Usage

```c
#include <sys/resource.h>

int getrusage(int who, struct rusage *res_usage);

struct rusage {
    struct timeval ru_utime; /* User CPU time used */
    struct timeval ru_stime; /* System CPU time used */
    long ru_maxrss; /* Maximum size of resident set (kilobytes) [used since Linux 2.6.32] */
    long ru_ixrss; /* Integral (shared) text memory size (kilobyte-seconds) [unused] */
    long ru_idrss; /* Integral (unshared) data memory used (kilobyte-seconds) [unused] */
    long ru_isrss; /* Integral (unshared) stack memory used (kilobyte-seconds) [unused] */
    long ru_minflt; /* Soft page faults (I/O not required) */
    long ru_majflt; /* Hard page faults (I/O required) */
    long ru_nswap; /* Swaps out of physical memory [unused] */
    long ru_inblock; /* Block input operations via file system [used since Linux 2.6.22] */
    long ru_oublock; /* Block output operations via file system [used since Linux 2.6.22] */
    long ru_msgsnd; /* IPC messages sent [unused] */
    long ru_msgrcv; /* IPC messages received [unused] */
    long ru_nsignals; /* Signals received [unused] */
    long ru_nvcsw; /* Voluntary context switches (process relinquished CPU before its time slice expired) [used since Linux 2.6] */
    long ru_nivcsw; /* Involuntary context switches (higher priority process became runnable or time slice ran out) [used since Linux 2.6] */
};
```

- `getrusage()`system call返回calling process或者其child process所使用的各种system resources的统计信息
- `RUSAGE_SELF` 返回calling process的信息
- `RUSAGE_CHILDREN` 返回calling process的所有被终止和处于等待状态下的child process的信息
- `RUSAGE_THREAD` 返回calling thread的相关信息，Linux特有属性

## Process Resource Limits

```c
#include <sys/resource.h>

int getrlimit(int resource, struct rlimit *rlim);
int setrlimit(int resource, const struct rlimit *rlim);
int prlimit(pid_t pid, int resource, const struct rlimit *new_limit, struct rlimit *old_limit);

struct rlimit {
    rlim_t rlim_cur; /* Soft limit (actual process limit) */
    rlim_t rlim_max; /* Hard limit (ceiling for rlim_cur) */
};
```

- `getrlimit()`和`setrlimit()`system calls允许一个process获取和修改自己的resource limit
- `soft limit(rlim_cur)` 规定了process可以消耗资源的数，process可以把soft limit调整为`0-hard limit`范围内的任何值
- `hard limit(rlim_max)` 对于多数resource来说，hard limit的唯一作用是为soft limit设置上限
- privileged process`CAP_SYS_RESOURCE`可以增加或者减少hard limit，前提是上限仍然大于soft limit，unprivileged process只能缩小hard
  limit，而且此操作是的不可逆的
- 如果为`rlim_cur`和`rlim_max`设置了`RLIM_INFINITY`，则表示无限制，即不限制资源的使用
- `prlimit()`为Linux特有system call，允许同时获取和设置resource limit，也可以为其他process做同样的操作

| resource          | Limit on                                                                      | SUSv3 |
|-------------------|-------------------------------------------------------------------------------|-------|
| RLIMIT_AS         | Process virtual memory size (bytes)                                           | •     |
| RLIMIT_CORE       | Core file size (bytes) •                                                      | •     |
| RLIMIT_CPU        | CPU time (seconds)                                                            | •     |
| RLIMIT_DATA       | Process data segment (bytes)                                                  | •     |
| RLIMIT_FSIZE      | File size (bytes)                                                             | •     |
| RLIMIT_MEMLOCK    | Locked memory (bytes)                                                         |       |
| RLIMIT_MSGQUEUE   | Bytes allocated for POSIX message queues for real user ID (since Linux 2.6.8) |       |
| RLIMIT_NICE       | Nice value (since Linux 2.6.12)                                               |       |
| RLIMIT_NOFILE     | Maximum file descriptor number plus one                                       | •     |
| RLIMIT_NPROC      | Number of processes for real user ID                                          |       |
| RLIMIT_RSS        | Resident set size (bytes; not implemented)                                    |       |
| RLIMIT_RTPRIO     | Realtime scheduling priority (since Linux 2.6.12)                             |       |
| RLIMIT_RTTIME     | Realtime CPU time (microseconds; since Linux 2.6.25)                          |       |
| RLIMIT_SIGPENDING | Number of queued signals for real user ID (since Linux 2.6.8)                 |       |
| RLIMIT_STACK      | Size of stack segment (bytes)                                                 | •     |

## Details of Specific Resource Limits

- `RLIMIT_AS` 指定了process的虚拟内存（地址空间）的最大字节(byte)数
- `RLIMIT_CORE` 指定了一个process在被特定signal终止之后产生的core dump文件的最大字节(byte)数
- `RLIMIT_CPU` 指定了process最多可以使用CPU的秒数，包括user mode和system mode
- `RLIMIT_DATA` 指定了process的data segment(`initialized data` + `uninitialized data` + `heap segments`)的最大字节(byte)数
- `RLIMIT_FSIZE` 指定了process可以创建的文件的最大字节(byte)数
- `RLIMIT_MEMLOCK`
- `RLIMIT_MSGQUEUE`
- `RLIMIT_NICE`
- `RLIMIT_NOFILE` 指定了process能够分配到的最大的file descriptor的数量 + 1
- `RLIMIT_NPROC` 指定了calling process的`real user ID`
  下最多可以创建process的数量，Linux实际指的是可以创建thread的数量，和`/proc/sys/kernel/threads-max`配置功能相同
- `RLIMIT_RSS`
- `RLIMIT_RTPRIO` 指定了使用`sched_setscheduler()`和`sched_setparam()`给process设置的实时优先级的最高值
- `RLIMIT_RTTIME` 指定了在realtime scheduling策略下运行的一个process在不休眠的情况下最多可以消耗的CPU时间
- `RLIMIT_SIGPENDING` 指定了calling process的real user ID对应的signal queue可以容纳signal的数量
- `RLIMIT_STACK` 指定了process stack的最大字节数