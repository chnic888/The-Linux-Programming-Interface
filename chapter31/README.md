# THREADS: THREAD SAFETY AND PER-THREAD STORAGE

## Thread Safety (and Reentrancy Revisited)
- `thread-safe` 一个函数同时可供多个thread安全调用
- 实现`thread-safe`函数的方式
    - 将函数和`mutex`关联使用，在调用时候锁定`mutex`，在返回时解锁`mutex`，同时只有一个thread可以访问此函数，因此这个函数的调用是`serialized`
    - 将一个`shared variable`和`mutex`关联使用，首先需要识别出函数内访问`shared variable`的`critical sections`，并且只在执行`critical sections`代码时才获取和释放`mutex`

## One-Time Initialization
```c
#include <pthread.h>

int pthread_once(pthread_once_t *once_control, void (*init)(void));
```
- `pthread_once()` 使用参数`once_control`的状态来保证，无论有多少不同的thread调用了多次`pthread_once()`，`init`所指向的函数仅会被执行一次
- `once_control`参数必须是指针，指向一个静态初始化值为`PTHREAD_ONCE_INIT`的变量
```c
pthread_once_t once_var = PTHREAD_ONCE_INIT;
```

## Thread-Specific Data
![31-1.png](./img/31-1.png)
- `thread-specific`数据允许函数为不同的thread分别维护一个变量的副本

### Thread-Specific Data from the Library Function’s Perspective
- 函数必须给每个调用他的thread单独分配一个存储块，thread第一次调用函数时候会分配存储块
- 后续相同thread对函数的调用，函数都应当返回第一次为thread分配的存储块
- 不同的函数都需要`thread-specific`数据，每个函数也需要方法标识出自己的`thread-specific`
- 因为函数无法控制当`thread`终止时发生的情况，因此需要一种机制能够自动的回收thread的存储块，以免造成内存泄露

### Overview of the Thread-Specific Data API
- `pthread_key_create()`来创建`key`，用以区分`pthread_key_create()`函数和其他函数使用的`thread-specific`数据项，`key`仅需在首个thread第一次调用`pthread_key_create()`的时候生成，因此可配合`pthread_once()`在使用
- `pthread_key_create()`可以定义`destructor`解构函数，thread终止时Pthreads AP会自动调用`destructor`解构函数并释放为`key`分配的存储块
- 函数给每个调用他的thread分配一个`thread-specific`数据块，这个分配通过`malloc()`或者类似函数来实现，分配数据块只会在每个thread第一次调用函数时分配
- `pthread_setspecific()`和`pthread_getspecific()`通过和之前的`key`结合，可以做到将指针存入或读出数据块，如果调用`pthread_getspecific()`时还没有特定的指针和`key`关联，函数会返回NULL

### Details of the Thread-Specific Data API
```c
#include <pthread.h>

int pthread_key_create(pthread_key_t *key, void (*destructor)(void *));
```
- `pthread_key_create()`返回一个新的`thread-specific`数据键，并且通过`key`指向的buffer返回给调用者
- `key`可以被process内的所有thread使用，因此`key`应当指向一个全局变量 
- 只要thread终止时`key`关联的值不为NULL，`destructor`指向的函数就会被自动执行，并将与`key`关联的值作为参数传入`destructor`函数中，如果无需解构函数，则将`destructor`置为NULL
- 如果一个thread有多个`thread-specific`数据块，无法确定解构函数调用的顺序，因此每个解构函数的设计应当相互独立

```c
#include <pthread.h>

int pthread_setspecific(pthread_key_t key, const void *value);
void *pthread_getspecific(pthread_key_t key);
```

### Employing the Thread-Specific Data API

### Thread-Specific Data Implementation Limits

## Thread-Local Storage