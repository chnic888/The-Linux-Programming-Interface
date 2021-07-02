# PIPES AND FIFOS

## Overview

### A pipe is a byte stream
- 在使用pipe时候是不存在消息或者消息边界概念的，从pipe中读取数据的process可以读取任意大小的数据块，不用关心写入process的数据块的大小
- 通过pipe传递的数据是顺序的，从pipe中读取出来的字节顺序和他们被写入pipe的顺序是完全一致的，因此如法在pipe中使用`lseek()`来随机访问数据

### Reading from a pipe
- 尝试从一个当前为空的pipe中读取数据将会被阻塞，直至至少有一个字节被写入到pipe中为止
- 一旦pipe的的最后一个写入端被关闭，消费端的process将会在读取完pipe剩余数据之后得到文件的结尾，即`read()`返回0

### Pipes are unidirectional
- pipe中的数据传输是单向的，一段用于写入，另外一段则用于读取

### Writes of up to PIPE_BUF bytes are guaranteed to be atomic
- 如果多个process同时写入一个pipe，在同一时刻写入的数据量不超过`PIPE_BUF`字节，则可以确保写入的数据不会发生混合的情况
- 当写入pipe的数据块大小超过了`PIPE_BUF`，那么kernel可能会将数据分割成几个较小的片段来传输，`write()`调用会阻塞直到所有数据被写入到pipe为止
- 如果只有一个process对pipe进行写入，`PIPE_BUF`的取值在这种情况下则不会有任何的影响。如果有多个process进行写入，大数据块有可能会被kernel分解成为小的数据段，并且出现写入process数据交叉的现象
- 当写入的数据大小到达`PIPE_BUF`字节时，`write()`会在有必要时阻塞，直到pipe中的可用空间足以原子性的完成操作

### Pipes have a limited capacity
- pipe简单来说就是kernel内存中维护的一个buffer，且buffer拥有最大值，一旦pipe被填满之后，后续对于pipe的写入将会被block，直到读取方从pipe中消费了一些数据为止

## Creating and Using Pipes
```c
#include <unistd.h>

int pipe(int filedes[2]);
```
- `pipe()`会在数组`filedes`中返回两个open file descriptors
	- `filedes[0]`为pipe的读取端的fd
	- `filedes[0]`为pipe的写入端的fd
	  
![44-2.png](./img/44-2.png)  

- parent process和child process通过pipe传输信息  

![44-3.png](./img/44-3.png)  

## Pipes as a Method of Process Synchronization

## Using Pipes to Connect Filters

## Talking to a Shell Command via a Pipe: popen()
```c
#include <stdio.h>

FILE *popen(const char *command, const char *mode);
int pclose(FILE *stream);
```
- `popen()`函数会创建一个pipe，之后会`fork()`一个child process来执行shell，而shell又会创建一个child process来执行`command`字符串

![44-4.png](./img/44-4.png)

## Pipes and stdio Buffering
- 当`mode`为`w`时来调用`popen()`
	- 默认情况下只有与stdio的buffer被填满或者调用了`pclose()`关闭pipe之后，输出才会被发送给pipe另外一端的child process，
	- 如果需要child process可以立刻接收到数据，则需要周期性的调用`fflush()`或者通过`setbuf(fp, NULL)`来禁用stdio的缓冲
- 当`mode`为`r`时来调用`popen()`
	- 如果另一端的child process使用stdio库，除非显示的调用`fflush()`或者`setbuf(fp, NULL)`，否则只有在child process填满stdio buff或者调用了`pclose()`关闭pipe之后才会数据才会可用

## FIFOs
```c
#include <sys/stat.h>

int mkfifo(const char *pathname, mode_t mode);
```

## A Client-Server Application Using FIFOs

## Nonblocking I/O

## Semantics of read() and write() on Pipes and FIFOs