# SIGNALS: SIGNAL HANDLERS

## Designing Signal Handlers
signal handler应该设计的足够简单来避免race condition的风险
- signal handler设置一个全局变量并退出
- signal handler执行某种类型的清理工作，之后终止进程或使用nonlocal goto来讲stack解除

### Reentrant and Async-Signal-Safe Functions
- `reentrant` 多个线程可以simultaneously的安全的调用某一个函数，那这个函数就是可以被称为`reentrant`
- `reentrant` SUSv3定义的`reentrant`，一个函数在多线程为定义顺序的情况下被调用，甚至是交叉调用，调用效果依然是一致的，no side affect

### Standard async-signal-safe functions

#### Reentrant and nonreentrant functions
`async-signal-safe` 一个`async-signal-safe`函数指的是其函数实现可以被signal handler安全的调用。如果一个函数是`async-signal-safe`的则代表函数要么是`reentrant`要么不会被signal handler打断
- 确保signal handler的代码本身是`reentrant`而且只调用`async-signal-safe`的方法
- 当主程序执行`async-signal-unsafe`的方法或者操作一个有可能被signal handler更新的数据结构时，要阻塞信号的传递。阻塞信号有些时候比较困难，所以规则可以简化为signal handler中绝不调用`async-signal-unsafe`的函数

#### Use of errno inside signal handlers
```c
void handler(int sig) {
 int savedErrno;
 savedErrno = errno;
 /* Now we can execute a function that might modify errno */
 errno = savedErrno;
}
```