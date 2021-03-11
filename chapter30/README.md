# THREADS: THREAD SYNCHRONIZATION

## Protecting Accesses to Shared Variables: Mutexes
- `critical section` 是指访问某一共享资源并且应当以`atomic`来执行的代码片段

### Statically Allocated Mutexes

### Locking and Unlocking a Mutex
```c
#include <pthread.h>

int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

### Performance of Mutexes

### Mutex Deadlocks

### Dynamically Initializing a Mutex
```c
#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
```

```c
#include <pthread.h>

int pthread_mutex_destroy(pthread_mutex_t *mutex);
```

### Mutex Attributes

### Mutex Types

## Signaling Changes of State: Condition Variables

### Statically Allocated Condition Variables

### Signaling and Waiting on Condition Variables
```c
#include <pthread.h>

int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
```

```c
#include <pthread.h>
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);
```

### Testing a Condition Variable’s Predicate

### Example Program: Joining Any Terminated Thread

### Dynamically Allocated Condition Variables
```c
#include <pthread.h>

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
```

```c
#include <pthread.h>

int pthread_cond_destroy(pthread_cond_t *cond);
```