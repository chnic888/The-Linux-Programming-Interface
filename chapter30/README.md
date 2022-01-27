# THREADS: THREAD SYNCHRONIZATION

## Protecting Accesses to Shared Variables: Mutexes

- `critical section` 是指访问某一共享资源并且应当以`atomic`来执行的代码片段
- 为了避免thread之间试图更新共享变量时所出的问题，必须使用`mutex(mutual exclusion)`来确保在同一时间只有一个thread可以访问某个共享资源，`mutex`可以确保对共享资源的原子访问
- 一个`mutex`会有两种状态`locked`和`unlocked`，在任何时刻至多只能有一个thread可以获取一个`mutex`上的锁，试图对一个`locked`状态的`mutex`上锁，讲可能引起阻塞或者失败报错
- 一旦`thread`对一个`mutex`锁定，随即就变成了`mutex`的所有者，也只有所有者才能对`mutex`进行解锁

![30-2.png](./img/30-2.png)

### Statically Allocated Mutexes

- `mutex`既可以被被分配成为一个静态变量，也可以在运行时动态创建，比如通过`malloc()`

```c
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
```

### Locking and Unlocking a Mutex

- 初始化之后的`mutex`处于`unlocked`状态，`pthread_mutex_lock()`和`pthread_mutex_unlock()`可以lock或者unlock一个`mutex`

```c
#include <pthread.h>

int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

- `pthread_mutex_lock()`参数`mutex`指定了需要锁定的mutex
	- 如果此时的mutex处于`unlocked`状态，则会锁定mutex并且立刻返回
	- 如果此时的mutex处于`locked`状态，也就是被另一thread锁定，则`pthread_mutex_lock()`会一直阻塞直至mutex被解锁为止
	- 如果一个thread已经持有一个mutex的锁，并且再次调用`pthread_mutex_lock()`，则根据mutex的默认类型会有两种结果：deadlock或者调用失败并把errno设置为`EDEADLK`
	  ，Linux默认使用deadlock策略
- 如果有多个thread试图锁定一个刚刚通过`pthread_mutex_unlock()`解锁的mutex，无法预测哪个thread可以获取锁

#### pthread_mutex_trylock() and pthread_mutex_timedlock()

```c
#include <pthread.h>

int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```

- 如果`mutex`当前是`locked`状态，对其使用`pthread_mutex_trylock()`则会失败并设置errno为`EBUSY`
- 对于mutex的良好设计应该是一个thread持有mutex的时间应当尽量的短，以避免妨碍其他thread并发执行，这也保证了因为`mutex`被blocked的thread可以立刻获取mutex

### Performance of Mutexes

- `file locks`和`semaphores`总是需要system call来执行lock和unlock的操作，每个system call虽然开销小但是仍是一个可感知的开销
- `mutexes`通过原子的机器语言级别的操作来实现，并且仅在需要锁征用的情况下才会发生system call
- Linux中，`mutexes`通过`futexes(fast user space mutexes)`机制来实现，锁的征用需要通过`futex()`system call

### Mutex Deadlocks

- 当多个thread试图锁定同一组`mutex`时，就有可能发生deadlock

![30-3.png](./img/30-3.png)

- 解决deadlock可能的方案
	- `mutex hierarchy` 定义一套`mutex`的层级关系，当多个thread对一组`mutex`操作时，应该总是以相同的顺序对这个`mutex`组进行锁定
	- `try, and then back off` thread先使用`pthread_mutex_lock()`锁定第一个mutex，然后使用`pthread_mutex_trylock()`锁定剩余的`mutexes`
	  ，如果任意的`pthread_mutex_trylock()`调用失败，则该thread释放所有属于自己的`mutex`

### Dynamically Initializing a Mutex

```c
#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
```

- `PTHREAD_MUTEX_INITIALIZER`只能用于对`mutex`进行静态默认属性的初始化，其他的场景需要使用`pthread_mutex_init()`来对`mutex`进行初始化
- 参数`mutex`指定了将要初始化的mutex
- `attr`是指向`pthread_mutexattr_t`类型对象的指针，该对象在函数调用之前已经经过了初始化，用于定义`mutex`的属性
- 如下情况下应当使用`pthread_mutex_init()`而不是使用静态分配的`mutex`
	- 动态分配于heap中的`mutex`
	- `mutex`是在stack中分配的自动变量
	- 静态分配且不使用默认属性的`mutex`

```c
#include <pthread.h>

int pthread_mutex_destroy(pthread_mutex_t *mutex);
```

- 如果一个自动或者动态分配的`mutex`不再被需要，应当使用`pthread_mutex_destroy()`将其销毁
- 只有一个`locked`状态的`mutex`，且后续没有thread试图锁定`mutex`，才能安全的把`mutex`销毁

### Mutex Attributes

### Mutex Types

- `PTHREAD_MUTEX_NORMAL` 不具有deadlock自检功能类型的`mutex`，如果thread试图对自己锁定的mutex加锁，则发生死锁
- `PTHREAD_MUTEX_ERRORCHECK` 对`mutex`所有的操作都会执行错误检查，此种`mutex`相比普通`mutex`慢，一般用于调试使用
- `PTHREAD_MUTEX_RECURSIVE` 一个递归的`mutex`维护了锁计数器，当thread第一次取得`mutex`
  则计数器被设为1，后续同一thread每次执行加锁操作都会增加计数器，解锁则会减少这个锁计数器，只有锁的计数器降为0时才会释放该`mutex`

## Signaling Changes of State: Condition Variables

- `condition variable` 条件变量允许一个thread向其他thread发送关于共享变量(或其他共享资源)状态变化的通知，并允许其他thread等待这种通知
- 一个条件变量总是应当结合着`mutex`来使用，条件变量根据`mutex`的状态改变发出通知

### Statically Allocated Condition Variables

- `condition variable`既可以被静态分配也可以被动态分配，条件变量的类型为`pthread_cond_t`

```c
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
```

### Signaling and Waiting on Condition Variables

- `condition variable`的主要操作是`signal`和`wait`
	- `signal operation`发送一个signal给一个或者多个thread，通知threads某个共享变量的状态已经改变
	- `wait operation`是指处于阻塞状态直至收到一个通知

```c
#include <pthread.h>

int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_broadcast(pthread_cond_t *cond);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
```

- `pthread_cond_signal()`和`pthread_cond_broadcast()`都可以根据`cond`指定的条件变量来发送signal
	- `pthread_cond_signal()` 只保证至少一个处于blocked状态的thread会被唤醒
	- `pthread_cond_broadcast()` 所有被blocked状态的thread会被唤醒
- `pthread_cond_wait()`将阻塞一个thread直到收到了条件变量`cond`的通知
- `pthread_cond_signal()`的使用场景
	- 不需要同时唤醒所有的thread，因此`pthread_cond_signal()`更为高效
	- 某一个thread被优先调度，此thread检查shared variable(s)状态并发现仍然有任务需完成，该thread完成了所需工作，并改变shared variable(s)表明任务完成并解锁相关的mutex
	- 每个剩余的thread轮流锁定`mutex`并且测试`shared variable`的状态
- `pthread_cond_broadcast()`使用场景
	- 处于等待状态的所有thread被设计成为执行不同的任务

```c
#include <pthread.h>
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);
```

- `pthread_cond_timedwait()`的`abstime`指定了在条件变量被signal通知之前处于休眠状态的thread的时间上限，如果超过了时间上限`abstime`
  且条件变量也并未收到通知，则返回错误`ETIMEDOUT`

#### Using a condition variable in the producer-consumer example

- `pthread_cond_wait()`总是需要结合着`condition variable`和`mutex`一起使用，之后`pthread_cond_wait()`会执行如下的操作
	- 解锁`mutex`指定的mutex，并**同时**阻塞calling thread，以等待另一thread给`cond`指向的`condition variable`发送signal
	- 收到另一thread向`condition variable`发送的signal之后，`pthread_cond_wait()`返回并重新锁定`mutex`
- `condition variable`和`mutex`之间的关系
	- thread锁定`mutex`并准备检查`predicate`，也就是`shared variable`是否满足某种状态
	- 检查`predicate`是否满足要求
	- 如果`predicate`不满足要求，calling thread会先解锁`mutex`然后在`condition variable`处休眠以等待signal，这里解锁`mutex`
	  +休眠是一个原子性的操作，也就是在calling thread在进入休眠之前，不可能有其他的thread获取`mutex`，因此也可不可能有thread发送`condition variable`的signal，calling
	  thread休眠之后其他的thread便可以访问`shared variable`，因为`mutext`此时已经处于`unlocked`的状态
	- 当thread因为`condition variable`被通知而被重新唤醒时，thread需要对`mutex`再次锁定，`pthread_cond_wait()`方法返回时`mutex`
	  已经被再次锁定，通常情况下被重新唤醒的thread会立刻访问`predicate`
	- `pthread_cond_wait()`中的`mutex`的作用并不是为了保护`condition variable`，而是为了保护被`condition variable`用来实现signal机制的`predicate`

### Testing a Condition Variable’s Predicate

- 必须使用while循环而不是if语句来控制对`pthread_cond_wait()`的调用，因为从`pthread_cond_wait()`返回时，并不能判断`predicate`的状态，因此应该立刻检查`predicate`
  ，并且在条件不满足时重新使得thread进入休眠，这么做的几种原因
- 给`condition variable`发送signal意味着“可能有些事情”需要接受signal并唤醒的thread去完成，而不是”一定有一些事情“要完成，因此需要重新判断`predicate`
- 有些实现中，有可能发生虚假唤醒，也就是没有任何thread给`condition variable`发送signal，处于等待的thread仍有可能被唤醒，因此需要重新判断`predicate`

### Dynamically Allocated Condition Variables

```c
#include <pthread.h>

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr);
```

- 自动和动态的初始化`condition variable`必须使用`pthread_cond_init()`
- 不采用默认属性的静态分配的`condition variable`也必须使用`pthread_cond_init()`

```c
#include <pthread.h>

int pthread_cond_destroy(pthread_cond_t *cond);
```

- `pthread_cond_destroy()`用来销毁自动或者动态分配的`condition variable`，`PTHREAD_COND_INITIALIZER`创建的静态初始化的`condition variable`
  无需调用`pthread_cond_destroy()`来销毁
- 一个`condition variable`在仅当没有thread在等待他的时，将其销毁才是安全的。
- 如果`condition variable`驻留在动态创建的内存区域，应该释放内存前就将其销毁。自动分配的`condition variable`应该在宿主函数返回前就应予销毁