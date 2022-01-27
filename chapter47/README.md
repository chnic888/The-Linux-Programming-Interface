# SYSTEM V SEMAPHORES

- System V的信号量不是用来process之间传输数据的，而是用来同步process之间的动作
- 信号量是由kernel维护的整数，其值被限制为大于或等于0，在一个信号量上可以执行如下的操作，且最后两个操作会导致calling process被阻塞
	- 将信号量设置成为一个绝对值
	- 给信号量当前的值加上一个数值
	- 给信号量当前的值减去一个数值
	- 等待一个信号量的值等于0，

- 当减少一个信号量的值时，kernel会把所有试图将信号量值减少到0之下的操作阻塞
- 如果一个信号量的值当前不等于0, 那么一个正在等待信号量的值等于0的calling process将会一直被kernel阻塞

![47-1.png](./img/47-1.png)

## Overview

- 使用System V的信号量的通常步骤如下
	- 使用`semget()`创建或者打开一个信号量集
	- 使用`semctl()`的`SETVAL`或`SETALL`操作初始化集合中的信号量，这个操作应该只有一个process来操作
	- 使用`semop()`操作信号量的值，process通常会使用信号量来获取或者脂肪一种共享资源
	- 当所有process都不需要信号量集之后，使用`semctl()`的`IPC_RMID`操作来删除这个集合

## Creating or Opening a Semaphore Set

```c
#include <sys/types.h> /* For portability */
#include <sys/sem.h>

int semget(key_t key, int nsems, int semflg);
```

- `semget()` 系统调用创建一个新的信号量或者获取一个已存在信号量集合的标识符
- `nsems`参数在创建信号量场景下，会指定集合中信号量的个数，其值必须大于0; 在获取信号量场景下，其值必须小于等于集合数量的大小，无法修改一个已存在信号量的数量
- `semflg`是一个bit mask，用来指定新创建的信号量或者检查既有信号量的权限
	- `IPC_CREAT` 如果没有与指定`key`相对应的信号量集合，则会创建一个新的信号量集合
	- `IPC_EXCL` 如果同时指定了`IPC_CREAT`和`IPC_EXCL`，如果对于指定`key`相应的信号量集合已经存在，那么会调用失败并返回`EEXIST`错误

## Semaphore Control Operations

```c
#include <sys/types.h> /* For portability */
#include <sys/sem.h>

int semctl(int semid, int semnum, int cmd, ... /* union semun arg */);
```

- `semctl()`系统调用在一个信号量集合或者集合中单个信号量上执行操作

### Generic control operations

- `IPC_RMID` 立即删除信号量以及关联的数据结构，任何在`semop()`调用中因为等待信号量而阻塞的process都会被立刻唤醒，`semop()`会返回`EIDRM`错误，且这个操作无需`arg`参数
- `IPC_STAT` 将与此信号量集关联的`semid_ds`数据结构的副本放入`arg.buf`指向的缓冲区中
- `IPC_SET` 使用`arg.buf`指向的缓冲区中的值更新与此信号量集关联的`semid_ds`数据结构中的所选定的字段

### Retrieving and initializing semaphore values

- `GETVAL` 返回由`semid`指定的信号量集中的第`semnum`个信号量的值，这个操作`arg`参数不是必需的
- `SETVAL` `semid`引用的集合中的第`semnum`信号量的值被初始化为`arg.val`中指定的值
- `GETALL` 检索`semid`引用的集合中所有信号量的值，将它们放置在`arg.array`指向的数组中
- `SETALL` 使用`arg.array`指向的数组中提供的值，初始化`semid`引用的集合中的所有信号量。

### Retrieving per-semaphore information

- `GETPID` 返回上一个在该信号量上执行`semop()`的process的pid，这个值被称为`sempid`值，如果还没有进程对该信号量执行`semop()`，那么就返回0
- `GETNCNT` 返回当前等待此信号量值增加的process数，称为`semncnt`值
- `GETZCNT` 返回当前等待该信号量值变为0的process数；这称为`semzcnt`值

## Semaphore Associated Data Structure

```c
struct semid_ds {   
    struct ipc_perm sem_perm; /* Ownership and permissions */   
    time_t sem_otime; /* Time of last semop() */
    time_t sem_ctime; /* Time of last change */
    unsigned long sem_nsems; /* Number of semaphores in set */
};
```

## Semaphore Initialization

## Semaphore Operations

```c
#include <sys/types.h> /* For portability */
#include <sys/sem.h>

int semop(int semid, struct sembuf *sops, unsigned int nsops);
```

```c
#define _GNU_SOURCE
#include <sys/types.h> /* For portability */
#include <sys/sem.h>

int semtimedop(int semid, struct sembuf *sops, unsigned int nsops, struct timespec *timeout);
```

## Handling of Multiple Blocked Semaphore Operations

## Semaphore Undo Values

## Implementing a Binary Semaphores Protocol

## Semaphore Limits

## Disadvantages of System V Semaphores