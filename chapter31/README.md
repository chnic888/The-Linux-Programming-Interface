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
- `pthread_once()` 使用参数`once_control`的状态来保证，无论有多少不同的thread调用了次`pthread_once()`，`init`所指向的函数仅会被执行一次
- `once_control`参数必须是指针，指向一个静态初始化值为`PTHREAD_ONCE_INIT`的变量
```c
pthread_once_t once_var = PTHREAD_ONCE_INIT;
```

## Thread-Specific Data
![31-1.png](./img/31-1.png)
- `thread-specific`数据允许函数为不同的thread分别维护一个变量的副本

### Thread-Specific Data from the Library Function’s Perspective

### Overview of the Thread-Specific Data API

### Details of the Thread-Specific Data API
```c
#include <pthread.h>

int pthread_key_create(pthread_key_t *key, void (*destructor)(void *));
```

```c
#include <pthread.h>

int pthread_setspecific(pthread_key_t key, const void *value);
void *pthread_getspecific(pthread_key_t key);
```
### Employing the Thread-Specific Data API

### Thread-Specific Data Implementation Limits

## Thread-Local Storage