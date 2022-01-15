# INTERPROCESS COMMUNICATION OVERVIEW

## A Taxonomy of IPC Facilities
- `communication` 关注process之间数据的交换
- `synchronization` 关注process或thread操作之间的同步
- `signals` 在某种情况下可作为一种同步技术，也可以通过在realtime signal上绑定数据来作为通信技术

![43-1.png](./img/43-1.png)

## Communication Facilities
- `data-transfer facilities` 一个process将数据写入到IPC的facility当中，而另一个process则会从中读取数据。这些facility会要求在user memory和kernel memory中进行两次数据传输
  - 第一次是写入process将数据从user memory写入到kernel memory中
  - 第二次是读取process将数据从kernel memory读取到user memory中
	
![43-2.png](./img/43-2.png)

- `shared memory` process通过将数据放到一片可以和其他process共享的内存区域当中，从而完成信息的交换
  - kernel通过将每个process的page-table条目指向同一个RAM的分页来实现共享内存
  - 一个process可以将数据放入到共享内存区域中，从而使得数据对其他process可用
  - 由于通信无需system calls以及user memory和kernel memory间的数据传输，因此共享内存是一种非常快速的通信机制

### Data transfer
- `byte stream` 通过pipes、FIFOs和datagram sockets交换的数据是一个无界的字节流。每次的读取操作都会从facility中读取任意数量的字节，而不管写入方写入的block的大小是多少
- `message` 通过System V消息队列、POSIX消息队列以及stream sockets交换的数据是一种有界的消息。每次读取操作都会读取写入process写入的一整条消息，无法只读取部分消息，也无法读取多条消息
- `pseudoterminals` 是一众特殊情况下使用的通讯facility

- `data-transfer facilities`和`shared memory`之间的区别
  - data-transfer可以有多个数据读取者，但是读取操作是有副作用的。一个读取操作会消耗数据，且读取之后数据会对其他读取者不可用
  - 读取和写入的操作是同步且原子性的，如果一个读取者试图从一个当前不包含数据的data-transfer facility中读取数据，读取操作会被阻塞，直到一些process向facility中写入数据

### Shared memory
- 大多数现代的Unix系统提供了三种形式的共享内存:`System V shared memory`、`POSIX shared memory`和`memory mappings`
- 共享内存需要考虑在内存上的同步操作，在共享内存中，`semaphore`通常被用作同步方法  
- 放入共享内存中的数据对所有共享这块内存的process可见

## Synchronization Facilities
- `semaphores` 一个`semaphore`是由一个内核维护的整数，且值永远不会小于0。一个process可以增加或减少一个`semaphore`的值，一个process试图将`semaphore`的值减小到小于0，那么kernel会阻塞该操作直至`semaphore`的值的增长允许执行该操作的程度
- `file locks` 设计用来协调多个process对同一文件的动作的一种同步方法。文件锁分为读共享锁和写排他锁，任意process都可持有同一文件的读锁，但当一个process持有了一个文件的写锁之后，其他process将无法获取该文件的读锁和写锁
- `mutexes and condition variables` 通常搭配POSIX threads一起使用

## Comparing IPC Facilities
| Facility type | Name used to identify object | Handle used to refer to object in programs |
| --- | --- | --- |
| Pipe | no name | file descriptor |
| FIFO | pathname | file descriptor |
| UNIX domain socket | pathname | file descriptor |
| Internet domain socket | IP address + port number | file descriptor |
| System V message queue | System V IPC key | System V IPC identifier |
| System V semaphore | System V IPC key | System V IPC identifier |
| System V shared memory | System V IPC key | System V IPC identifier |
| POSIX message queue | POSIX IPC pathname | mqd_t (message queue descriptor) |
| POSIX named semaphore | POSIX IPC pathname | sem_t * (semaphore pointer) |
| POSIX unnamed semaphore | no name | sem_t * (semaphore pointer) |
| POSIX shared memory | POSIX IPC pathname | file descriptor |
| Anonymous mapping | no name | none |
| Memory-mapped file | pathname file descriptor |
| flock() lock | pathname | file descriptor |
| fcntl() lock | pathname | file descriptor |

### Functionality
- `data-transfer facilities`提供了读取和写入操作，传输的数据只提供一个reader process进行消费
- `shared-memory` 其他application更适合采用的模式，一个process通过`shared-memory`能够使得数据对共享同一内存区域的所有process可见

### Network communication
- 只有`socket`允许process通过网络通信。`socket`一般用于两个domain
  - `UNIX domain` 允许统一系统上的process进行通信
  - `Internet domain` 允许通过TCP/IP网络进行连接的不同主机上的process进行通信

### Portability
- 从可移植性上来看，System V IPC要优于POSIX IPC