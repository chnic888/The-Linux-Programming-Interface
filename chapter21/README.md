# SIGNALS: SIGNAL HANDLERS

## Designing Signal Handlers
signal handler应该设计的足够简单来避免race condition的风险
- signal handler设置一个全局变量并退出
- signal handler执行某种类型的清理工作，之后终止进程或使用nonlocal goto来讲stack解除

### Reentrant and Async-Signal-Safe Functions
- `reentrant` 多个线程可以simultaneously的安全的调用某一个函数，那这个函数就是可以被称为`reentrant`
- `reentrant` SUSv3定义的`reentrant`，一个函数在多线程为定义顺序的情况下被调用，甚至是交叉调用，调用效果依然是一致的，no side affect