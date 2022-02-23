# INTRODUCTION-TO-POSIX-IPC

- `POSIX IPC`和`System V IPC`一样，也包含`message queues`，`semaphores`和`shared memory`机制
	- `message queues` 允许process之间传递有边界的消息，队列允许给每个消息分配一个优先级，队列中高优先级的消息会排在地优先级的消息前面
	- `semaphores` 允许多个process同步各自的动作，也是一个由kernel维护的单个整数，并且永远不会小于0
	- `shared memory` 允许多个process共享同一块内存区域，一旦一个process更新了内存之后，变更立刻对共享同一区域的其他process可见

## API Overview

| Interface                | Message queues                                                                                 | Semaphores                                                                          | Shared memory                         |
|--------------------------|------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------|---------------------------------------|
| Header file              | <mqueue.h>                                                                                     | <semaphore.h>                                                                       | <sys/mman.h>                          |
| Object handle            | mqd_t                                                                                          | sem_t *                                                                             | int (file descriptor)                 |
| Create/open              | mq_open()                                                                                      | sem_open()                                                                          | shm_open() + mmap()                   |
| Close                    | mq_close()                                                                                     | sem_close()                                                                         | munmap()                              |
| Unlink                   | mq_unlink()                                                                                    | sem_unlink()                                                                        | shm_unlink()                          |
| Perform IPC              | mq_send()<br/>mq_receive()                                                                     | sem_post()<br/>sem_wait()<br/>sem_getvalue()                                        | operate on locations in shared region |
| Miscellaneous operations | mq_setattr()—set attributes<br/>mq_getattr()—get attributes<br/>mq_notify()—request attributes | sem_init()—initialize unnamed semaphore<br/>sem_destroy()—destroy unnamed semaphore | (none)                                |

### IPC object names

- SUSv3指定标识POSIX IPC对象的唯一可移植方式，即字符串使用协线打头，后跟一个或者多个非斜线字符

### Creating or opening an IPC object

### Closing an IPC object

- IPC对象在process终止或执行`exec()`时会自动关闭

### IPC object permissions

- IPC对象上的`permissions mask`和文件的`permissions mask`是一致的，对于POSIX IPC对象来说，执行权限是没有意义的

### IPC object deletion and object persistence

- 与打开的文件一样，POSIX IPC对象是引用计数的——kernel维护对对象的打开引用数的计数，与System V IPC对象相比，使的应用程序更容易确定何时可以安全删除对象
- 每个POSIX IPC对象都有一个相应的`unlink()`调用，其操作类似于对文件的传统`unlink()`syscall，`unlink()`调用立即删除对象的名称，然后在所有process停止使用它，即引用计数降至零时销毁该对象
	- 对于消息队列和信号量，对象在所有process都关闭对象后被销毁
	- 对于共享内存，销毁发生在所有process使用`munmap()`取消mapping对象之后

### Listing and removing POSIX IPC objects via the command line

### Compiling programs that use POSIX IPC on Linux

- 在Linux上，使用POSIX消息队列和共享内存的程序必须使用`cc –lrt`选项与运行时库`librt`链接，使用POSIX信号量的程序必须使用`cc –pthread`选项进行编译。

## Comparison of System V IPC and POSIX IPC

### POSIX IPC advantages

- POSIX IPC接口比System V IPC接口更加简单
- POSIX IPC模型——使用名称而不是key，以及`open`、`close`、`unlink`功能——更符合传统的UNIX文件模型
- POSIX IPC对象是引用计数的，简化了对象删除，因为可以`unlink`一个POSIX IPC对象，知道只有当所有process都关闭时IPC对象才会被销毁

## System V IPC advantages

- 可以移植性更好，System V IPC在SUSv3中指定，几乎所有UNIX的实现都支持
- 尽管POSIX IPC对象名称有SUSv3规范，但各种实现仍然会遵循不同的IPC对象命名约定
- POSIX IPC的各种细节没有在SUSv3中规定，有其实没有规定用于显示和删除系统上已经存在的IPC对象的命令