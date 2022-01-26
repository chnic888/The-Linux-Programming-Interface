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

## Semaphore Control Operations
```c
#include <sys/types.h> /* For portability */
#include <sys/sem.h>

int semctl(int semid, int semnum, int cmd, ... /* union semun arg */);
```

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