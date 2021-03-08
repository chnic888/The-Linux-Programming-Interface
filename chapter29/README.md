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

- 在multithreaded程序中，每个thread都有属于自己的errno
- Pthreads API的所有Pthreads functions均返回`0`表示成功，返回positive error number表示失败

## Thread Creation
```c
#include <pthread.h>

int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start)(void *), void *arg);
```

## Thread Termination
```c
include <pthread.h>
        
void pthread_exit(void *retval);
```

## Thread IDs
```c
include <pthread.h>
        
pthread_t pthread_self(void);
```

```c
include <pthread.h>
        
int pthread_equal(pthread_t t1, pthread_t t2);
```

## Joining with a Terminated Thread
```c
include <pthread.h>
        
int pthread_join(pthread_t thread, void **retval);
```

## Detaching a Thread
```c
#include <pthread.h>

int pthread_detach(pthread_t thread);
```

## Thread Attributes

## Threads Versus Processes