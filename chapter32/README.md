# THREADS: THREAD CANCELLATION

## Canceling a Thread
```c
#include <pthread.h>

int pthread_cancel(pthread_t thread);
```

## Cancellation State and Type
```c
#include <pthread.h>

int pthread_setcancelstate(int state, int *oldstate);
int pthread_setcanceltype(int type, int *oldtype);
```

## Cancellation Points

## Testing for Thread Cancellation
```c
#include <pthread.h>

void pthread_testcancel(void);
```

## Cleanup Handlers
```c
#include <pthread.h>

void pthread_cleanup_push(void (*routine)(void*), void *arg);
void pthread_cleanup_pop(int execute);
```

## Asynchronous Cancelability