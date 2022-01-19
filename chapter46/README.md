# SYSTEM V MESSAGE QUEUES
- 消息队列允许process以消息的方式来交换数据，在某些方面和FIFO相似，但是仍然有一些差别
  - 用来引用消息队列的句柄是由一个由`msgget()`调用返回的标识符
  - 通过消息队列的通信是面面向消息的，reader接收到的是由writer写入的整条消息，而FIFO是byte stream
  - 除了包含数据之外，每条消息还包含一个整型的字段的`type`，即从消息队列中读取消息既可以按照先入先出的顺序，也可根据类型来读取消息

## Creating or Opening a Message Queue
```c
#include <sys/types.h> /* For portability */
#include <sys/msg.h>

int msgget(key_t key, int msgflg);
```
- `msgget()`system call创建一个新的消息队列或者取得一个已存在的队列的标识符
- `msgflg`参数是一个bit mask，用来指定放置在新的消息队列或者检查既有消息队列的权限
  - `IPC_CREAT` 如果没有与指定`key`相对应的消息队列，则会创建一个新的消息队列
  - `IPC_EXCL` 如果人同时指定了`IPC_CREAT`和`IPC_EXCL`，如果于指定`key`相应的队列已经存在，那么会调用失败并返回`EEXIST`错误

## Exchanging Messages

### Sending Messages
```c
#include <sys/types.h> /* For portability */
#include <sys/msg.h>

int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
```

### Receiving Messages
```c
#include <sys/types.h> /* For portability */
#include <sys/msg.h>

ssize_t msgrcv(int msqid, void *msgp, size_t maxmsgsz, long msgtyp, int msgflg);
```

## Message Queue Control Operations
```c
#include <sys/types.h> /* For portability */
#include <sys/msg.h>

int msgctl(int msqid, int cmd, struct msqid_ds *buf);
```

## Message Queue Associated Data Structure
```c
struct msqid_ds {
	struct ipc_perm msg_perm; /* Ownership and permissions */
	time_t msg_stime; /* Time of last msgsnd() */
 	time_t msg_rtime; /* Time of last msgrcv() */
 	time_t msg_ctime; /* Time of last change */
 	unsigned long __msg_cbytes; /* Number of bytes in queue */
 	msgqnum_t msg_qnum; /* Number of messages in queue */
 	msglen_t msg_qbytes; /* Maximum bytes in queue */
 	pid_t msg_lspid; /* PID of last msgsnd() */
 	pid_t msg_lrpid; /* PID of last msgrcv() */
};
```

## Message Queue Limits

## Displaying All Message Queues on the System

## Client-Server Programming with Message Queues

## A File-Server Application Using Message Queues

## Disadvantages of System V Message Queues