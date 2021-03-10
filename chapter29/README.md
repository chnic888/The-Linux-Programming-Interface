# THREADS: INTRODUCTION

## Overview
- 一个process可以包含多个thread，同一个程序中所有的thread都会执行相同的program，并且共享同一份全局内存区域，包括`initialized data` `uninitialized data`和`heap segments`
- 传统意义上的Unix的process只不过是一个multithreaded process的特例，即该process只包含一个thread

![29-1.png](./img/29-1.png)

- multiple process的一些限制
    - process之间的信息难以共享，parent和child process只会共享只读的`text segment`，必须采用IPC的方式来在process间交换信息
    - 调用`fork()`创建process代价相对比较昂贵
- multiple thread的优点
    - thread之间可以方便、快速的共享信息，只需要将数据复制到共享的全局或者heap变量中即可
    - 创建thread比创建process通常要快10倍，Linux上通过`clone()`system call来实现thread

## Background Details of the Pthreads API

### Pthreads data types
| Data type | Description |
| --- | --- |
| pthread_t | Thread identifier |
| pthread_mutex_t | Mutex |
| pthread_mutexattr_t | Mutex attributes object |
| pthread_cond_t | Condition variable |
| pthread_condattr_t | Condition variable attributes object |
| pthread_key_t | Key for thread-specific data |
| pthread_once_t | One-time initialization control context |
| pthread_attr_t | Thread attributes object |

### Threads and errno
- 在multithreaded程序中，每个thread都有属于自己的errno
- Pthreads API的所有Pthreads functions均返回`0`表示成功，返回positive error number表示失败

## Thread Creation
```c
#include <pthread.h>

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start)(void *), void *arg);
```
- 新thread通过调用带有`arg`参数的`start()`开始执行，`arg`参数为`void *`类型，可以指向任何对象类型的指针并传递给`start()`函数，一般情况下`arg`指针指向一个global或heap变量，如果需要向`start()`传递多个参数，可以将`arg`指向一个struct
- 参数`thread`指向`pthread_t`类型的buffer，在`pthread_create()`函数`return`之前会把一个thread的唯一标识符拷贝进buffer中。SUSv3明确规定在新thread开始执行之前，不需要初始化`thread`所指向的buffer，新`thread`可能在`pthread_create()`函数return之前就开始执行，因此只能使用`pthread_self()`来获取自己的ID 
- 参数`attr`是指向`pthread_attr_t`对象的指针，如果设置为NULL，将使用默认属性    

## Thread Termination
- thread终止的方法
    - thread的`start`函数执行了`return`语句并返回指定值
    - thread调用了`pthread_exit()`
    - 使用`pthread_cancel()`取消了thread    
    - 任意thread调用了`exit()`或者main thread执行了`return`语句，所有process中的threads都会立刻终止
```c
include <pthread.h>

void pthread_exit(void *retval);
```
- 调用`pthread_exit()`相当于在thread中执行了`return`语句
- 参数`retval`指定了thread的返回值，`retval`所指向的值不应分配在`thread stack`上，`thread stack`上的内容在thread终止的时候失效
- 如果main thread调用了`pthread_exit()`，不会影响其他thread的继续执行

## Thread IDs
```c
include <pthread.h>

pthread_t pthread_self(void);
```
- `Thread ID(TID)` process内的每一个thread的唯一标识符，每个thread可以通过`pthread_self()`来获取自己的TID

```c
include <pthread.h>

int pthread_equal(pthread_t t1, pthread_t t2);
```
- `pthread_equal()`函数用来检查两个thread的ID是否相同，在Linux的threading实现中，TID在所有的process中都是唯一的
- `pthread_t`在不同的实现中定义是不一致的，因此必须使用`pthread_equal()`来判断是否相等，以满足最大的可移植性

## Joining with a Terminated Thread
```c
include <pthread.h>

int pthread_join(pthread_t thread, void **retval);
```
- `pthread_join()`会等待`thread`指定的thread终止，一旦thread终止，`pthread_join()`会解除阻塞立刻返回，这种操作被称之为`joining`(连接)
- `retval`如果为一个非NULL的指针，将会收到一个thread终止时候返回值的拷贝，这个返回值是thread调用`return`或者`pthread_exit()`指定的值
- 如果thread并未`detached`，则必须使用`pthread_join()`来进行连接，如果连接失败则thread终止之后会转变成为`zombie thread`，从而浪费系统资源

## Detaching a Thread
```c
#include <pthread.h>

int pthread_detach(pthread_t thread);
```

## Thread Attributes

## Threads Versus Processes