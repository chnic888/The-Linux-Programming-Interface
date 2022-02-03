# SYSTEM V MESSAGE QUEUES

- System V的消息队列允许process以消息的方式来交换数据，在某些方面和FIFO相似，但是仍然有一些差别
	- 用来引用消息队列的句柄是由一个由`msgget()`调用返回的标识符
	- 通过消息队列的通信是面面向消息的，reader接收到的是由writer写入的整条消息，而FIFO是byte stream
	- 除了包含数据之外，每条消息还包含一个整型的字段的`type`，即从消息队列中读取消息既可以按照先入先出的顺序，也可根据类型来读取消息

## Creating or Opening a Message Queue

```c
#include <sys/types.h> /* For portability */
#include <sys/msg.h>

int msgget(key_t key, int msgflg);
```

- `msgget()`系统调用创建一个新的消息队列或者取得一个已存在的队列的标识符
- `key`参数通常是值`IPC_PRIVATE`或者`ftok()`的返回的key
- `msgflg`参数是一个bit mask，用来指定放置在新的消息队列或者检查既有消息队列的权限，还可以通过下述一个或多个标记取OR来控制操作
    - `IPC_CREAT` 如果没有与指定`key`相对应的消息队列，则会创建一个新的消息队列
    - `IPC_EXCL` 如果同时指定了`IPC_CREAT`和`IPC_EXCL`，如果对于指定`key`相应的队列已经存在，那么会调用失败并返回`EEXIST`错误

## Exchanging Messages

- `msgsnd()`和`msgrcv()`系统调用执行消息队列上的I/O，两个系统调用的第一参数`msqid`是消息队列的标识符，第二个参数`msgp`为程序自定义的结构，用来存放发送和接收的消息，通常为

```c
struct mymsg {  
    long mtype; /* Message type */
    char mtext[]; /* Message body */
}
```

### Sending Messages

```c
#include <sys/types.h> /* For portability */
#include <sys/msg.h>

int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
```

- `msgsnd()`系统调用向消息队列里写入一条消息，调用时`msgp`结构内的`mtype`必须是一个大于0的值
- `msgp`结构中的`mtext`代表需要传递的消息，`msgsz`参数指定了`mtext`字段中包含的字节数
- `msgflg`是一个bit mask标志用于控制`msgsnd()`的操作
	- `IPC_NOWAIT` 执行一个非阻塞操作，`msgsnd()`不会被阻塞且会立刻返回`EAGAIN`错误

### Receiving Messages

```c
#include <sys/types.h> /* For portability */
#include <sys/msg.h>

ssize_t msgrcv(int msqid, void *msgp, size_t maxmsgsz, long msgtyp, int msgflg);
```

- `msgrcv()`系统调用从消息队列中读取和移除一条消息，并且把消息复制进`msgp`所指向的缓冲区中
- `msgp`中的`mtext`字段的最大可用空间是通过`maxmsgsz`参数来指定的，如果队列中待删除的消息大小超过了`maxmsgsz`，那么就不会从队列中删除消息，并且`msgrcv()`会返回`E2BIG`错误
- 读取消息的顺序无需和消息被发送的顺序保持一致，可以根据`mtype`字段值来选择消息，这个选择是通过`msgflg`来决定的
	- 如果`msgtyp`等于0，那么会删除队列中的第一条消息并将其返回给calling process
	- 如果`msgtyp`大于0，那么会将队列中第一条`mtype`等于`msgtyp`的消息删除并将其返回给calling process
	- 如果`msgtyp`小于0，那么会将等待中的消息当成优先级队列来处理，队列中`mtype`最小且值小于等于`msgtyp`绝对值的第一条消息会被删除并返回给calling process
- `msgflg`参数是一个bit mask，会使用零个或多个值取OR来确定
	- `IPC_NOWAIT` 执行一个非阻塞的接收，指定`IPC_NOWAIT`标记会导致`msgrcv()`立即返回`ENOMSG`错误
	- `MSG_EXCEPT` 只有当`msgtyp`大于0时才会起作用，是一个补充的操作，即将队列的第一条`mtype`不等于`msgtyp`的消息删除并将其返回给调用者
	- `MSG_NOERROR` `msgrcv()`将会从队列中删除消息并将其`mtext`字段的大小截短为`maxmsgsz`字节，并将消息返回给调用者，被截取的数据将会丢失

## Message Queue Control Operations

```c
#include <sys/types.h> /* For portability */
#include <sys/msg.h>

int msgctl(int msqid, int cmd, struct msqid_ds *buf);
```

- `msgctl()`系统调用在标识符为`msqid`的消息队列上执行控制操作
- `cmd`指定了在队列上的操作
	- `IPC_RMID` 立即删除消息队列和关联的`msqid_ds`数据结构，队列中所有剩余消息都会丢失，任何被阻塞的reader和writer都会被立刻唤醒，`msgsnd()`和`msgrcv()`会失败并返回`EIDRM`
	  错误
	- `IPC_STAT` 将与此消息队列关联的`msqid_ds`数据结构的副本放在 buf 指向的缓冲区中。
	- `IPC_SET` 使用`buf`指向的缓冲区中提供的值来更新与此消息队列关联的`msqid_ds`数据结构所选定的字段。

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

- 每个消息队列都有一个关联的`msqid_ds`数据结构
- `msqid_ds`结构的字段由各种消息队列系统调用隐式更新，特定字段可以使用`msgctl()`的`IPC_SET`操作显式更新

## Message Queue Limits

- `MSGMNI` 系统级别的限制，规定了系统中能创建的消息队列标识符的数量
- `MSGMAX` 系统级别的限制，规定了单条消息中最大可写入的字节数`mtext`
- `MSGMNB` 系统级别的限制，规定了一个消息队列中一次最多可保存的字节数`mtext`
- `MSGTQL` 系统级别的限制，规定了系统中所有消息队列能存放消息的总数
- `MSGPOOL` 系统级别的限制，规定了用于容纳系统上所有消息队列中数据的缓冲池的大小

```shell
cd /proc/sys/kernel

cat msgmni
cat msgmax
cat msgmnb
```

- Linux特有的`msgctl()` `IPC_INFO`操作能获取一个类型为`msginfo`的结构，包含了各种消息队列的限制的值

```c
struct msginfo buf;

msgctl(0, IPC_INFO, (struct msqid_ds *) &buf);
```

## Displaying All Message Queues on the System

- Linux可以通过特有的IPC ctl操作`msgctl()` `semctl()` `shmctl()`获取系统中所有IPC对象列表

## Client-Server Programming with Message Queues

## A File-Server Application Using Message Queues

## Disadvantages of System V Message Queues

- Unix系统为统一系统上的不同进程提供了多种机制
	- `undelimited byte stream` 无分隔符的字节流形式`pipes` `FIFOs` `UNIX domain stream sockets`
	- `delimited messages` 可分割的消息形式`System V message queues` `POSIX message queues` 和 `UNIX domain datagram sockets`