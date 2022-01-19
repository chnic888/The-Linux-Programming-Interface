# INTRODUCTION TO SYSTEM V IPC
- System V包括三种不同的IPC方式
  - `Message queues` 和pipe类似，但是存在两处不同
    - 消息边界被保留，以便读取和写入方通过消息单元而不是无边界的字节流来通信
    - 消息包含一个整形的`type`字段，并可以通过`type`来选择消息，而不是按照写入顺序来读取消息
  - `Semaphores` 一个信号量是一个由kernel维护的整数值，他对所有具备相应权限的process都可见
  - `Shared memory` 允许process共享同一块内存区域，是速度最快的一种IPC方法 

## API Overview
| Interface | Message queues | Semaphores |  Shared memory |
| --- | --- | --- | --- |
| Header file | <sys/msg.h> | <sys/sem.h> | <sys/shm.h> |
| Associated data structure | msqid_ds | semid_ds | shmid_ds |
| Create/open object | msgget() | semget() | shmget() + shmat() |
| Close object | (none) | (none) | shmdt() |
| Control operations | msgctl() | semctl() | shmctl() |
| Performing IPC | msgsnd()—write message msgrcv()—read message | semop()—test/adjust semaphore | access memory in shared region |

### Creating and opening a System V IPC object
- 每个System V IPC机制都有一个相关的get系统调用`msgget()``semget()``shmget()`，和`open()`调用类似，提供一个整型参数`key`
  - 使用给定的`key`创建一个新的IPC对象并返回一个唯一标识符来标识该对象
  - 使用给定的`key`返回一个已经存在的IPC对象的唯一标识符
- IPC标识符类似于file descriptor，在后续所有对于该IPC对象的引用都需要用到IPC标识符，不同的是file descriptor是一个process属性，而IPC标识符则是IPC对象本身的一个属性并在系统范围内可见

### IPC object deletion and object persistence
- 对于消息队列和信号量来说，IPC对象的删除会立刻生效，对象中包含的所有信息都会被销毁，不管其他的process是否任然在使用这个IPC对象
- 对于删除共享内存对象来说，在`shmctl(id, IPC_RMID, NULL)`调用之后，只有当所有使用该内存segment的`process`和该内存segment分离`shmdt()`之后，共享内存对象才会被删除
- System V IPC对象具有kernel持久性，一旦被创建之后会一直存在直到它被显示地删除或者系统被关闭

##  IPC Keys
- System V IPC key是一个整数值，其数据类型为`key_t`，IPC get调用将一个key转换成为相应的整型的IPC标识符 

### Generating a unique key with IPC_PRIVATE
- 在创建一个新IPC对象时，`key`被指定成为`IPC_PRIVATE`，并且无需指定`IPC_CREAT`或`IPC_EXCL`标记
```c
id = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR);
```
- 适用场景，parent process在执行`fork()`之前通过`IPC_PRIVATE`创建IPC对象，从而导致child process继承IPC对象标识符

### Generating a unique key with ftok()
```c
#include <sys/ipc.h>

key_t ftok(char *pathname, int proj);
```
- `ftok()`方法返回一个供后续System V IPC`get`调用的key的值

## Associated Data Structure and Object Permissions
- kernel为System V IPC对象的每个实例都维护者一个关联的数据结构，并且这个数据结构根据IPC的机制(消息队列、信号量或共享内存)的不同而不同
- 一个IPC对象关联的数据结构会通过相应的`get`system call创建的时进行初始化

## IPC Identifiers and Client-Server Applications

## Algorithm Employed by System V IPC get Calls

## The ipcs and ipcrm Commands
- 在默认情况下`ipcs`会显示出每个对象的key，标识符，所有者以及权限
- `ipcrm`命令删除一个IPC对象，常见的使用形式有如下的两种
```shell
ipcrm -X key
ipcrm -x id
```

## Obtaining a List of All IPC Objects
- `/proc/sysvipc/msg` 列出所有消息队列及其属性
- `/proc/sysvipc/sem` 列出所有信号两集及其属性
- `/proc/sysvipc/shm` 列出所有共享内存段及其属性

## IPC Limits