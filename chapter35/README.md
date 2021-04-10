# PROCESS PRIORITIES AND SCHEDULING

## Process Priorities (Nice Values)
- `round-robin time-sharing model` Linux和大多数Unix下，process scheduling使用CPU的默认模型。此模型下，每个process会轮流的使用CPU一段时间，这段时间被称为`time slice`或`quantum`
    - `Fairness` 公平性，即每个process都有机会获取到`time slice`
    - `Responsiveness` 响应度，一个process在接收CPU的使用权之前无需等待太长的时间
- 在`round-robin time-sharing`算法中，process无法直接控制还何时使用CPU以及使用CPU的时间，默认情况下，每个process轮流使用CPU直到`time slice`被用光或者process自己自动放弃CPU的使用权
- `nice value` process的属性，允许process间接影响kernel的调度算法，每个process都有一个nice value，取值范围为`-20 ～ +19`，其中`-20`为高优先级，`+19`为低优先级，默认值为`0`

![35-1.png](./img/35-1.png)

- 在传统的Unix实现中，只有`privileged processes`才能给自己赋予一个更高的优先级，`unprivileged processes`只能降低自己的优先级
- 使用`fork()`创建child process会继承`nice value`值，并且该值会在`exec()`调用中得到保持

### Effect of the nice value
- 给一个process赋低优先级(即高nice value)并不会导致process完全无法使用CPU，但是导致process使用CPU的时间变少

### Retrieving and modifying priorities
```c
#include <sys/resource.h>

int getpriority(int which, id_t who);
int setpriority(int which, id_t who, int prio);
```
- `PRIO_PROCESS` 操作PID等于`who`的process，如果`who`等于0，将会使用calling process的PID
- `PRIO_PGRP` 操作PGID等于`who`的process group内的所有成员，如果`who`等于0，将会使用calling process的process group
- `PRIO_USER` 操作process的`real user ID`等于`who`的process，如果`who`等于0，将会使用calling process的`real user ID`
- `getpriority()`system call返回由`which`和`who`所指定的nice value，如果有多个process符合标准(`which=PRIO_PGRP` or `which=PRIO_USER`)，那么会返回优先级最高的nice value
- `setpriority()`system call会将`which`和`who`所指定的nice value设置为`prio`，如果`prio`超过`–20 ～ +19`的范围，则会将nice value设置为边界值
- privileged process`CAP_SYS_NICE`能够修改任意process的优先级
- Linux Kernel 2.6.12之后，提供了`RLIMIT_NICE`资源限制，允许unprivileged process可以把自己的nice value提升到`20 ～ RLIMIT_NICE`范围内的值
- unprivileged process可以使用`setpriority()`来修改target process的nice value，条件是calling process的`effective user ID`必须要和target process的`real user ID`或`effective user ID`相等

## Overview of Realtime Process Scheduling

### The SCHED_RR Policy

### The SCHED_FIFO Policy

### The SCHED_BATCH and SCHED_IDLE Policies

## Realtime Process Scheduling API

### Realtime Priority Ranges

### Modifying and Retrieving Policies and Priorities

### Relinquishing the CPU

### The SCHED_RR Time Slice

## CPU Affinity
