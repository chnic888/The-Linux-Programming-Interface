# PROCESS CREATION AND PROGRAM EXECUTION IN MORE DETAIL

## Process Accounting
- `process accounting` kernel会在每个process终止时写入一条`accounting`记录到系统级别的`process accounting file`
- `accounting record` kernel为相应的process维护的多种信息，包括终止状态、CPU时间等

### Enabling and disabling process accounting
```c
#define _BSD_SOURCE
#include <unistd.h>

int acct(const char *acctfile);
```

### Process accounting records
- 一旦`process accounting`被打开，每当一个process被终止，就会有一条`acct`的记录被写入`process accounting file`中
- `acct` 结构定义在`<sys/acct.h>`中

```c
typedef u_int16_t comp_t; /* See text */
struct acct {
    char ac_flag; /* Accounting flags (see text) */
    u_int16_t ac_uid; /* User ID of process */
    u_int16_t ac_gid; /* Group ID of process */
    u_int16_t ac_tty; /* Controlling terminal for process (may be 0 if none, e.g., for a daemon) */
    u_int32_t ac_btime; /* Start time (time_t; seconds since the Epoch) */
    comp_t ac_utime; /* User CPU time (clock ticks) */
    comp_t ac_stime; /* System CPU time (clock ticks) */
    comp_t ac_etime; /* Elapsed (real) time (clock ticks) */
    comp_t ac_mem; /* Average memory usage (kilobytes) */
    comp_t ac_io; /* Bytes transferred by read(2) and write(2) (unused) */
    comp_t ac_rw; /* Blocks read/written (unused) */
    comp_t ac_minflt; /* Minor page faults (Linux-specific) */
    comp_t ac_majflt; /* Major page faults (Linux-specific) */
    comp_t ac_swaps; /* Number of swaps (unused; Linux-specific) */
    u_int32_t ac_exitcode; /* Process termination status */
#define ACCT_COMM 16
    char ac_comm[ACCT_COMM+1]; /* (Null-terminated) command name (basename of last execed file) */
    char ac_pad[10]; /* Padding (reserved for future use) */
};
```

### Process accounting Version 3 file format
-  kernel 2.6.8之后引入了`acct_v3`结构，需要在编译kernel前启用kernel配置选项`CONFIG_BSD_PROCESS_ACCT_V3`
```c
struct acct_v3 {
    char ac_flag; /* Accounting flags */
    char ac_version; /* Accounting version (3) */
    u_int16_t ac_tty; /* Controlling terminal for process */
    u_int32_t ac_exitcode; /* Process termination status */
    u_int32_t ac_uid; /* 32-bit user ID of process */
    u_int32_t ac_gid; /* 32-bit group ID of process */
    u_int32_t ac_pid; /* Process ID */
    u_int32_t ac_ppid; /* Parent process ID */
    u_int32_t ac_btime; /* Start time (time_t) */
    float ac_etime; /* Elapsed (real) time (clock ticks) */
    comp_t ac_utime; /* User CPU time (clock ticks) */
    comp_t ac_stime; /* System CPU time (clock ticks) */
    comp_t ac_mem; /* Average memory usage (kilobytes) */
    comp_t ac_io; /* Bytes read/written (unused) */
    comp_t ac_rw; /* Blocks read/written (unused) */
    comp_t ac_minflt; /* Minor page faults */
    comp_t ac_majflt; /* Major page faults */
    comp_t ac_swaps; /* Number of swaps (unused; Linux-specific) */
#define ACCT_COMM 16
    char ac_comm[ACCT_COMM]; /* Command name */
};
```

## The clone() System Call
```c
#define _GNU_SOURCE
#include <sched.h>

int clone(int (*func) (void *), void *child_stack, int flags, void *func_arg, ... /* pid_t *ptid, struct user_desc *tls, pid_t *ctid */ );
```
- `clone()`是Linux的特有system call，基于threading库来实现，因此会有损程序的可移植性
- `clone()`生成的child process会去执行`func`指定的函数，因此`func`又被成为`child function`，调用参数由`func_arg`指定
- `func`返回或者调用`exit()`或`_exit()`之后，`clone()`system call产生的child process则会被终止，`func`的返回值为process的退出状态，parent process可以使用`wait()`系列函数来等待child process
- `child_stack`指向child process需要使用的stack空间，因为大多数硬件架构中，stack是向下增长的，所以`child_stack`指针应该指向内存块的高位
- `clone()`如果成功创建child process，返回值为child pid，如果创建失败则返回-1
- `flags`的低字节指定了child process的`termination signal`，这个signal会在child process被终止时传递给parent process，如果`clone()`产生的child process被signal**停止**，`parent process`则仍然会收到`SIGCHLD`signal

| Flag | Effect if present |
| --- | --- |
| CLONE_CHILD_CLEARTID | Clear ctid when child calls exec() or _exit() (2.6 onward) |
| CLONE_CHILD_SETTID | Write thread ID of child into ctid (2.6 onward) |
| CLONE_FILES | Parent and child share table of open file descriptors |
| CLONE_FS | Parent and child share attributes related to file system |
| CLONE_IO | Child shares parent’s I/O context (2.6.25 onward) |
| CLONE_NEWIPC | Child gets new System V IPC namespace (2.6.19 onward) |
| CLONE_NEWNET | Child gets new network namespace (2.4.24 onward) |
| CLONE_NEWNS | Child gets copy of parent’s mount namespace (2.4.19 onward) |
| CLONE_NEWPID | Child gets new process-ID namespace (2.6.19 onward) |
| CLONE_NEWUSER | Child gets new user-ID namespace (2.6.23 onward) |
| CLONE_NEWUTS | Child gets new UTS (utsname()) namespace (2.6.19 onward) |
| CLONE_PARENT | Make child’s parent same as caller’s parent (2.4 onward) |
| CLONE_PARENT_SETTID | Write thread ID of child into ptid (2.6 onward) |
| CLONE_PID | Obsolete flag used only by system boot process (up to 2.4) |
| CLONE_PTRACE | If parent is being traced, then trace child also |
| CLONE_SETTLS | tls describes thread-local storage for child (2.6 onward) |
| CLONE_SIGHAND | Parent and child share signal dispositions |
| CLONE_SYSVSEM | Parent and child share semaphore undo values (2.6 onward) |
| CLONE_THREAD | Place child in same thread group as parent (2.4 onward) |
| CLONE_UNTRACED | Can’t force CLONE_PTRACE on child (2.6 onward) |
| CLONE_VFORK | Parent is suspended until child calls exec() or _exit() |
| CLONE_VM | Parent and child share virtual memory |

### The clone() flags Argument

### Extensions to waitpid() for Cloned Children

## Speed of Process Creation

## Effect of exec() and fork() on Process Attributes