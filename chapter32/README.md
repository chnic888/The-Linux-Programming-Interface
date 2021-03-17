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
- 如果thread处于一个不包含取消点计算密集型的循环代码中，则无法被取消
- `pthread_testcancel()`会产生一个取消点，取消请求处于pending状态的thread调用此函数之后，就会被立即终止
- 当代码不包含取消点时，需要对`pthread_testcancel(void)`进行周期性的调用，以确保对pending的取消请求作出相应

## Cleanup Handlers
- 每个`thread`都可以添加一个或者多个清理函数，并在thread被取消时候自动执行这些函数，清理函数可以执行一些清理动作，比如修改全局变量，解锁`mutex`等操作
- 每个`thread`都有一个清理函数栈，清理函数会以后进先出的原则从栈顶开始执行
- 如果`thread`顺利执行完所有代码并备有被取消，则清理函数则不会被调用

```c
#include <pthread.h>

void pthread_cleanup_push(void (*routine)(void*), void *arg);
void pthread_cleanup_pop(int execute);
```
- `pthread_cleanup_push()`会将参数`routine`指向的方法添加到calling thread的清理函数栈的栈顶，`arg`指向的参数会被当成参数传递给`routine`指向的方法
- `pthread_cleanup_pop()`会将清理函数栈的栈顶函数弹出并执行，如果参数`execute`为非0，函数即便在thread被非取消情况下也会被执行，`pthread_cleanup_pop()`伴随`pthread_cleanup_push()`调用
- thread的清理函数栈如果有未弹出的函数，如果调用`pthread_exit()`会自动所未弹出的函数会被弹出并执行，如果正常`return`返回则不会调用这些清理函数

## Asynchronous Cancellability
- 一个异步取消thread`PTHREAD_CANCEL_ASYNCHRONOUS`，可以被随时取消，取消动作不会等到下一个取消点才会被触发
- 一个可以被异步取消的thread不应当分配任何资源，也不能获取比如`mutexes` `semaphores`或`locks`，典型的使用场景就是取消计算密集型循环的thread