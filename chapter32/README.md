# THREADS: THREAD CANCELLATION

## Canceling a Thread
```c
#include <pthread.h>

int pthread_cancel(pthread_t thread);
```
- `pthread_cancel()`向参数`thread`制定的thread发送一个取消的请求，请求发送后`pthread_cancel()`会立刻返回，不会等待目标thread的退出

## Cancellation State and Type
```c
#include <pthread.h>

int pthread_setcancelstate(int state, int *oldstate);
```
- `PTHREAD_CANCEL_DISABLE` thread不可被取消，假如此类thread收到取消请求，thread会一直pending直到状态变为`PTHREAD_CANCEL_ENABLE`，临时启用此种状态，可以保证thread代码完整的被执行，不会因为取消而被打断
- `PTHREAD_CANCEL_ENABLE` thread可被取消，这是新thread的默认属性
- `oldstate` 返回之前的取消状态，应当总是为`oldstate`设置一个非NULL的值，以保证可移植性

```c
#include <pthread.h>

int pthread_setcanceltype(int type, int *oldtype);
```
- 如果thread的取消`state`为`PTHREAD_CANCEL_ENABLE`，对于取消请求的处理取决于thread的取消`type`
- `PTHREAD_CANCEL_ASYNCHRONOUS` 可能会在任何时间点取消thread
- `PTHREAD_CANCEL_DEFERRED` 取消请求处于pending，直到thread内的程序运行到了最近的取消点，这是新thread的默认属性
- `oldtype` 返回之前的取消类型，应当总是为`oldtype`设置一个非NULL的值，以保证可移植性

- thread调用`fork()`时，取消`state`和`type`会被child继承
- thread调用`exec()`时，新程序的main thread的`state`和`type`会被重置为默认值

## Cancellation Points
- `PTHREAD_CANCEL_ENABLE`加`PTHREAD_CANCEL_DEFERRED`设置下，取消请求只有到达取消点时，取消请求才会生效
- thread到达取消点被终止之后，如果thread仍然是可连接状态，必须使用`pthread_join()`来防止thread变为`zombie thread`，`pthread_join()`返回的第二个参数将会是一个特殊值`PTHREAD_CANCELED`

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