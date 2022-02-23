# POSIX-MESSAGE-QUEUES

- POSIX消息队列允许process之间以消息的形式来交换数据，和System V消息队列类似的是数据交换单位是整个消息
	- POSIX消息队列使用引用计数，只有所有当前使用队列的process都关闭队列后，之前被标记为删除的队列才会被移除
	- POSIX的消息有一个关联的优先级，消息之前是严格按照优先级顺序排序的，而System V的每个消息都有一个整型的字段`type`
	- POSIX消息队列中的一条消息可用时，可以异步的通知process

## Overview

- POSIX消息队列主要函数如下
	- `mq_open()` 创建一个新消息队列或者打开一个已有的消息队列，并返回后续调用中会使用到的消息队列描述符
	- `mq_send()` 向队列中写入一条消息
	- `mq_receive()` 从队列中读取一条消息
	- `mq_close()` 关闭process之前打开的一个消息队列
	- `mq_unlink()` 删除一个消息队列名，并当所有process关闭该队列时候对队列进行标记以便删除
	- `mq_getattr()` 获取消息队列属性
	- `mq_setattr()` 修改消息队列属性
	- `mq_notify()` 允许一个process向一个队列注册消息通知，注册完成之后当一条消息可用时，会发送一个signal或在一个单独的线程中调用一个函数来通知process

## Opening, Closing, and Unlinking a Message Queue

```c
#include <fcntl.h> 		/* Defines O_* constants */
#include <sys/stat.h> 	/* Defines mode constants */
#include <mqueue.h>

mqd_t mq_open(const char *name, int oflag, .../* mode_t mode, struct mq_attr *attr */);
```

- `mq_open()` 创建一个新消息队列或者打开一个已有的消息队列
- `name` 用来标识消息队列，即字符串使用协线打头，后跟一个或者多个非斜线字符
- `oflag` 是一个bit mask，用来确定是创建一个新队列还是打开一个已有队列，还可以指定calling process在消息队列上的访问方式
	- `O_NONBLOCK` 以非阻塞方式打开队列，如果`mq_send()`或`mq_receive()`无法在非阻塞的情况下执行，那么会立刻返回`EAGAIN`错误

| Flag                             | Description                                                                         |
|----------------------------------|-------------------------------------------------------------------------------------|
| O_CREAT<br/>O_EXCL               | Create queue if it doesn’t already exist<br/>With O_CREAT, create queue exclusively |
| O_RDONLY<br/>O_WRONLY<br/>O_RDWR | Open for reading only<br/>Open for writing only<br/>Open for reading and writing    |
| O_NONBLOCK                       | Open in nonblocking mode                                                            |

- 如果`oflag`中指定了`O_CREAT`，那么就需要另外两个参数`mode`和`attr`
	- `mode` 是一个bit mask没，用来指定新消息队列上的权限
	- `attr` 是一个`mq_attr`结构，制定了新的消息队列的特性，如果为NULL，则使用实现定义的默认属性来创建队列

### Effect of fork(), exec(), and process termination on message queue descriptors

- `fork()`期间，child process会接收parent process消息队列描述符的副本，这些描述符引用相同的打开消息队列描述，child process不会继承其parent process的任何消息通知注册
- 当process执行`exec()`或终止时，所有之前打开的消息队列描述符都将关闭，并且在相应队列上的消息通知注册都将被注销

### Closing a message queue

```c
#include <mqueue.h>

int mq_close(mqd_t mqdes);
```

- `mq_close()` 关闭消息队列描述符`mqdes`，如果calling process之前通过`mqdes`注册过消息通知，则通知会被自动删除，随后另一个process可以注册队列中的消息通知
- 与文件的`close()`一样，关闭消息队列并不会删除它，删除需要使用`mq_unlink()`，是`unlink()`在的消息队列上的版本

### Removing a message queue

```c
#include <mqueue.h>

int mq_unlink(const char *name);
```

- `mq_unlink()` 删除通过`name`标识的消息队列，并把队列标记为当所有process停止使用该队列后将其销毁

## Relationship Between Descriptors and Message Queues

## Message Queue Attributes

```c
struct mq_attr {	
    long mq_flags; 		/* Message queue description flags: 0 or O_NONBLOCK [mq_getattr(), mq_setattr()] */
    long mq_maxmsg; 	/* Maximum number of messages on queue [mq_open(), mq_getattr()] */
    long mq_msgsize; 	/* Maximum message size (in bytes) [mq_open(), mq_getattr()] */
    long mq_curmsgs; 	/* Number of messages currently in queue [mq_getattr()] */
};
```

### Setting message queue attributes during queue creation

### Retrieving message queue attributes

```c
#include <mqueue.h>

int mq_getattr(mqd_t mqdes, struct mq_attr *attr);
```

### Modifying message queue attributes

```c
#include <mqueue.h>

int mq_setattr(mqd_t mqdes, const struct mq_attr *newattr, struct mq_attr *oldattr);
```

## Exchanging Messages

### Sending Messages

```c
#include <mqueue.h>

int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio);
```

### Receiving Messages

```c
#include <mqueue.h>

ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio);
```

### Sending and Receiving Messages with a Timeout

```c
#define _XOPEN_SOURCE 600
#include <mqueue.h>
#include <time.h>

int mq_timedsend(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned int msg_prio, const struct timespec *abs_timeout);

ssize_t mq_timedreceive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned int *msg_prio, const struct timespec *abs_timeout);
```

## Message Notification

```c
#include <mqueue.h>

int mq_notify(mqd_t mqdes, const struct sigevent *notification);
```

### Receiving Notification via a Signal

### Receiving Notification via a Thread

## Linux-Specific Features

## Message Queue Limits

## Comparison of POSIX and System V Message Queues