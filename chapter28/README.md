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
- `thread`和`process`属于KSE`kernel scheduling entity`，只是对比其他SKEs提供了或多或少属性共享上的不同

#### Sharing file descriptor tables: CLONE_FILES
- 如果设置`CLONE_FILES`，parent process和child process会共享同一个`open file descriptors table`，因此对一个process的fd的分配和释放，比如`open()` `close()` `dup()` `pipe()` `socket()`，都会对其他process可见
- 如果不设置`CLONE_FILES`，child process只是获取了parent process的`open file descriptors table`的副本
- `POSIX threads`规范要求process中的所有thread共享相同的`open file descriptors table`

#### Sharing file system–related information: CLONE_FS
- 如果设置`CLONE_FS`，parent process和child process会共享同文件系统相关信息，因此对一个process的`umask()` `chdir()` `chroot()`会影响其他process
- `POSIX threads`规范要求实现`CLONE_FS`提供的属性共享

#### Sharing signal dispositions: CLONE_SIGHAND
- 如果设置`CLONE_SIGHAND`，parent process和child process会共享相同的`signal dispositions table`，无论在哪个process调用`sigaction()`或`signal()`修改signal的disposition，都会影响其他process
- 如果不设置`CLONE_SIGHAND`，child process只是获取了parent process的`signal dispositions table`的副本
- `CLONE_SIGHAND`不会影响parent和child process之间的`signal mask`和`pending signals set`，parent和child process之间是彼此独立的  
- `POSIX threads`规范要求共享`signal dispositions`

#### Sharing the parent’s virtual memory: CLONE_VM
- 如果设置`CLONE_VM`，parent process和child process会共享同一份`virtual memory pages`，无论在哪个process调用`mmap()`或`munmap()`，都会影响其他process
- 如果不设置`CLONE_VM`，child process只是获取了parent process的`virtual memory pages`的拷贝  
- `POSIX threads`规范也对thread之间共享`virtual memory pages`有要求

#### Thread groups: CLONE_THREAD
- 如果设置`CLONE_THREAD`，child会被置于parent的thread group中
- 如果不设置`CLONE_THREAD`，child会被置于新的thread group中
- POSIX规定process所有的thread共享同一个pid，即不同thread调用`getpid()`都会返回相同值，`thread group`就是共享同一个`thread group identifier (TGID)`的一组KSE

![28-1.png](./img/28-1.png)

- 一个`thread group`内每个thread都有一个唯一的`thread identifier(TID)`，thread可以通过`gettid()`system call来获取自己的`TID`
- `TID`和`PID`的数据类型都为`pid_t`，且`TID`在系统级别是唯一的，除了一个thread是process内`thread group leader`的情况之外，kernel可以保证系统不会出现`TID`**等于**`PID`的情况
- `thread group`内的首个`thread`即为`thread group leader`，其`TID`等于`TGID`，也等于`PID`
- `thread group`内的所有thread都有一个相同的`parent process id(PPID)`， 即为`thread group leader`的`PPID`，`thread group`内的所有thread都终止以后，parent process才会收到`SIGCHLD`signal
- 如果一个通过`CLONE_THREAD`标志创建的thread被终止，不会有signal会发给该thread的创建者(创建者通过`clone()`创建)。POSIX thread不同于process，需要通过`pthread_join()`而不是`wait()`来检测是否终止
- 如果`thread group`内的任意thread调用了`exec()`，那么除了`thread group leader`之外的threads将会被终止，新的program将会在`thread group leader`中执行
- 如果`thread group`内的任意thread调用了`fork()`或`vfork()`来创建child，则组内任何的thread都可以使用`wait()`或类似函数来监控child
- Linux 2.6开始，如果设置了`CLONE_THREAD`，也必须同时设置`CLONE_SIGHAND`

#### Threading library support: CLONE_PARENT_SETTID, CLONE_CHILD_SETTID, and CLONE_CHILD_CLEARTID
- 如果设置`CLONE_PARENT_SETTID`，kernel会将`CTID`写入`pitd`指向的位置，在对parent process复制之前，就会把`CTID`拷贝到`ptid`的位置。可以保证即便没有设置`CLONE_VM`，parent thread和child thread均可以在此位置获得child thread id 
- 设置`CLONE_PARENT_SETTID`，可以保证在`clone()`返回之前就将新的`CTID`赋值给`ptid`指针，从来避免race conditions
- 如果设置`CLONE_CHILD_SETTID`，`clone()`会将`CTID`写入`ctid`指向的位置，并且对`ctid`的设置只会存在于child内存当中，如果设置了`CLONE_VM`，还是会影响到parent
- 如果设置`CLONE_CHILD_CLEARTID`，那么`clone()`会在child终止时将`ctid`所指向的内存清零，借助`CLONE_CHILD_CLEARTID`，NPTL threading implementation可获取thread的终止通知
- 通过`pthread_create()`创建一个thread时，NPTL会使用`clone()`，并且`ptid`和`ctid`都指向同一个位置，当child终止时，由于设置了`CLONE_CHILD_CLEARTID`，`ctid`被清除，并且这个清除动作对process内的所有thread都是可见的(由于设置了`CLONE_VM`)
- kernel将`ctid`指向的位置当做`futex`，`futex`为一种有效的同步机制，执行`futex()`system call会block的等待`ctid`所指向区域的变化，因此也可以获得thread终止的通知(`pthread_join()`的实现原理)。kernel清除cit的同时，也会唤醒任意的因为执行了`futex()`而被blocked的KSE

#### Thread-local storage: CLONE_SETTLS
- 如果设置`CLONE_SETTLS`，参数`tls`所指向thead级别使用的`thread-local`存储缓冲区的结构`user_desc`，此为NPTL为了支持thread-local storage的实现，在Linux 2.6之后加入的flag

#### Sharing System V semaphore undo values: CLONE_SYSVSEM
- 如果设置`CLONE_SYSVSEM`，parent和child共享同一个`System V semaphore undo values list`
- 如果不设置`CLONE_SYSVSEM`，parent和child各自独立的`undo list`，且child的列表为初始为空

#### Per-process mount namespaces: CLONE_NEWNS
- 默认情况下，parent和child共享同一个mount namespace，任意一个process调用了`mount()`和`umount()`改变了mount namespace，也会被其他process所见

#### Making the child’s parent the same as the caller’s: CLONE_PARENT
- 如果设置`CLONE_PARENT`，那么`child process.PPID = calling clone() process.PPID`
- 如果不设置`CLONE_PARENT`，那么`child process.PPID = calling clone() process.PID`
- child process的终止，会向`child process.PPID`所指向的parent process发出signal

#### Making the child’s PID the same as the parent’s PID: CLONE_PID (obsolete)
- 如果设置`CLONE_PID`，child pid则会等于他的parent的pid

#### Process tracing: CLONE_PTRACE and CLONE_UNTRACED
- 如果设置`CLONE_PTRACE`并且在在trace calling process，那么child process也会被traced

#### Suspending the parent until the child exits or execs: CLONE_VFORK
- 如果设置`CLONE_VFORK`，parent将一直被挂起直到child调用了`exec()`或`_exit()`来释放虚拟内存资源

#### New clone() flags to support containers
- 对于容器提供支持的flags `CLONE_IO` `CLONE_NEWIPC` `CLONE_NEWNET` `CLONE_NEWPID` `CLONE_NEWUSER` `CLONE_NEWUTS`

#### Use of clone() flags
- `fork()` 约等于flags指定为`SIGCHLD`的`clone()`调用
- `vfork()` 等于flags指定为`CLONE_VM | CLONE_VFORK | SIGCHLD`的`clone()`调用
- `LinuxThreads threading`的实现使用flags指定为`CLONE_VM | CLONE_FILES | CLONE_FS | CLONE_SIGHAND`的`clone()`调用
- `NPTL threading`的实现使用flags指定为`CLONE_VM | CLONE_FILES | CLONE_FS | CLONE_SIGHAND | CLONE_THREAD | CLONE_SETTLS | CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID | CLONE_SYSVSEM`的`clone()`调用

### Extensions to waitpid() for Cloned Children
- 通过`clone()`产生的child process，可以通过`waitpid()` `wait3()` `wait4()`的掩码参数`options`附加如下值
    - `__WCLONE` 如果设置只会等待`clone()`产生的child process，不设置则张炯会等待**非**`clone()`产生的child process
    - `__WALL` 等待所有的child process，不区分是否通过`clone()`产生
    - `__WNOTHREAD` 只等待calling process自己的child process，`waitpid()`不能使用此标志

## Speed of Process Creation
- `fork()` process所占的内存越大，`fork()`所需的时间也就越长，因为要复制页表，以及将`data` `heap` `stack segment page entries`标记为只读
- `vfork()` process的的内存大小不会影响`vfork()`的时间，因为`vfork()`不会拷页表和页，因此calling process的虚拟内存大小不会对其有影响

## Effect of exec() and fork() on Process Attributes
| Process attribute | `exec()` | `fork()` | Interfaces affecting attribute; additional notes |
| --- | --- | --- | --- |
| **_Process address space_** |  |  |  |
| Text segment | No | Shared | Child process shares text segment with parent. |
| Stack segment | No | Yes | Function entry/exit; alloca(), longjmp(), siglongjmp(). |
| Data and heap segments | No | Yes | brk(), sbrk(). |
| Environment variables | See notes | Yes | putenv(), setenv(); direct modification of environ. Overwritten by execle() and execve() and preserved by remaining exec() calls. |
| Memory mappings | No | Yes;see notes | mmap(), munmap(). A mapping’s MAP_NORESERVE flag is inherited across fork(). Mappings that have been marked with madvise(MADV_DONTFORK) are not inherited across fork(). |
| Memory locks | No | No | mlock(), munlock() |
| **_Process identifiers and credentials_** |  |  |  |
| Process ID | Yes | No | |
| Parent process ID | Yes | No | |
| Process group ID | Yes | Yes | setpgid(). |
| Session ID | Yes | Yes | setsid(). |
| Real IDs | Yes | Yes | setuid(), setgid(), and related calls. |
| Effective and saved set IDs | See notes | Yes | setuid(), setgid(), and related calls. Chapter 9 explains how exec() affects these IDs. |
| Supplementary group IDs | Yes | Yes | setgroups(), initgroups(). |
| **_Files, file I/O, and directories_** |  |  |  |
| Open file descriptors | See notes | Yes | open(), close(), dup(), pipe(), socket(), and so on. File descriptors are preserved across exec() unless marked close-on-exec. Descriptors in child and parent refer to same open file descriptions; see Section 5.4. |
| Close-on-exec flag | Yes (if off) | Yes | fcntl(F_SETFD). |
| File offsets | Yes | Shared | lseek(), read(), write(), readv(), writev(). Child shares file offsets with parent. |
| Open file status flags | Yes | Shared | open(), fcntl(F_SETFL). Child shares open file status flags with parent. |
| Asynchronous I/O operations | See notes | No | aio_read(), aio_write(), and related calls. Outstanding operations are canceled during an exec(). |
| Directory streams | No | Yes; see notes | opendir(), readdir(). SUSv3 states that child gets a copy of parent’s directory streams, but these copies may or may not share the directory stream position. On Linux, the directory stream position is not shared. |
| **_File system_** |  |  |  |
| Current working directory | Yes | Yes | chdir(). |
| Root directory | Yes | Yes | chroot(). |
| File mode creation mask | Yes | Yes | umask(). |
| **_Signals_** |  |  |  |
| Signal dispositions | See notes | Yes | signal(), sigaction(). During an exec(), signals with dispositions set to default or ignore are unchanged; caught signals revert to their default dispositions. See Section 27.5. |
| Signal mask | Yes | Yes | Signal delivery, sigprocmask(), sigaction(). |
| Pending signal set | Yes | No | Signal delivery; raise(), kill(), sigqueue(). |
| Alternate signal stack | No | Yes | sigaltstack(). |
| **_Timers_** |  |  |  |
| Interval timers | Yes | No | setitimer(). |
| Timers set by alarm() | Yes | No | alarm(). |
| POSIX timers | No | No | timer_create() and related calls. |
| **_POSIX threads_** |  |  |  |
| Threads | No | See notes | During fork(), only calling thread is replicated in child. |
| Thread cancelability state and type | No | Yes | After an exec(), the cancelability type and state are reset to PTHREAD_CANCEL_ENABLE and PTHREAD_CANCEL_DEFERRED, respectively |
| Mutexes and condition variables | No | Yes | See Section 33.3 for details of the treatment of mutexes and other thread resources during fork(). |
| **_Priority and scheduling_** |  |  |  |
| Nice value | Yes | Yes | nice(), setpriority(). |
| Scheduling policy and priority | Yes | Yes | sched_setscheduler(), sched_setparam(). |
| **_Resources and CPU time_** |  |  |  |
| Resource limits | Yes | Yes | setrlimit(). |
| Process and child CPU times | Yes | No | As returned by times(). |
| Resource usages | Yes | No | As returned by getrusage(). |
| **_Interprocess communication_** |  |  |  |
| System V shared memory segments | No | Yes | shmat(), shmdt(). |
| POSIX shared memory | No | Yes | shm_open() and related calls. |
| POSIX message queues | No | Yes | mq_open() and related calls. Descriptors in child and parent refer to same open message queue descriptions. A child doesn’t inherit its parent’s message notification registrations. |
| POSIX named semaphores | No | Shared | sem_open() and related calls. Child shares references to same semaphores as parent. |
| POSIX unnamed semaphores | No | See notes | sem_init() and related calls. If semaphores are in a shared memory region, then child shares semaphores with parent; otherwise, child has its own copy of the semaphores. |
| System V semaphore adjustments | Yes | No | semop(). See Section 47.8. |
| File locks | Yes | See notes | flock(). Child inherits a reference to the same lock as parent. |
| Record locks | See notes | No | fcntl(F_SETLK). Locks are preserved across exec() unless a file descriptor referring to the file is marked close-on-exec; see Section 55.3.5. |
| **_Miscellaneous_** |  |  |  |
| Locale settings | No | Yes | setlocale(). As part of C run-time initialization, the equivalent of setlocale(LC_ALL, “C”) is executed after a new program is execed. |
| Floating-point environment | No | Yes | When a new program is execed, the state of the floating-point environment is reset to the default; see fenv(3). |
| Controlling terminal | Yes | Yes |  |
| Exit handlers | No | Yes | atexit(), on_exit(). |
| **_Linux-specific_** |  |  |  |
| File-system IDs | See notes | Yes | setfsuid(), setfsgid(). These IDs are also changed any time the corresponding effective IDs are changed. |
| timerfd timers | Yes | See notes | timerfd_create(); child inherits file descriptors referring to same timers as parent. |
| Capabilities | See notes | Yes | capset(). The handling of capabilities during an exec() is described in Section 39.5. |
| Capability bounding set | Yes | Yes |
| Capabilities securebits flags | See notes | Yes | All securebits flags are preserved during an exec() except SECBIT_KEEP_CAPS, which is always cleared. |
| CPU affinity | Yes | Yes | sched_setaffinity(). |
| SCHED_RESET_ON_FORK | Yes | No | See Section 35.3.2. |
| Allowed CPUs | Yes | Yes | See cpuset(7). |
| Allowed memory nodes | Yes | Yes | See cpuset(7). |
| Memory policy | Yes | Yes | See set_mempolicy(2). |
| File leases | Yes | See notes | fcntl(F_SETLEASE). Child inherits a reference to the same lease as parent. |
| Directory change notifications | Yes | No | The dnotify API, available via fcntl(F_NOTIFY). |
| prctl(PR_SET_DUMPABLE) | See notes | Yes | During an exec(), the PR_SET_DUMPABLE flag is set, unless execing a set-user-ID or set-group-ID program, in which case it is cleared. |
| prctl(PR_SET_PDEATHSIG) | Yes | No |  |
| prctl(PR_SET_NAME) | No | Yes |  |
| oom_adj | Yes | Yes | See Section 49.9. |
| coredump_filter | Yes | Yes | See Section 22.1. |