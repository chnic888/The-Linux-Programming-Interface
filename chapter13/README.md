# FILE I/O BUFFERING

## Kernel Buffering of File I/O: The Buffer Cache
- system call尽管比磁盘操作要快很多，但system call所花费的时间也是相当可观的。kernel必须捕获调用，检查system call参数有效性并且在user space和kernel space间传输数据
- 与文件发生大量数据传输的情况下，通过采用大块空间来缓存数据，并且执行尽可能少的system call，就可以极大的提升I/O的整体性能

## Buffering in the stdio Library
### Setting the buffering mode of a stdio stream
```c
#include <stdio.h>

int setvbuf(FILE *stream, char *buf, int mode, size_t size);
```
- 在打开流之后，setvbuf()必须在调用stdio内任意方法之前被调用，setvbuf()会影响所有后续stdio内的所有方法
- 如果buf不为NULL，指向size大小的内存块将会作为stream的缓冲区，且内存是被静态或者动态分配在heap上的
- 如果buf为NULL，那么stdio library会自动分配一个缓冲区
- setvbuf()出错时返回非0值

mode代表了缓冲类型，支持如下值
- _IONBF 不对I/O进行缓冲，stdio library内的function将会立刻调用read() or write() system call.
- _IOLBF 行级别缓冲，在输入一个换行符前对数据进行缓冲，除非缓冲区已满。对于输入流，每次只读取一行数据。
- _IOFBF 采用全缓冲，单次的read()和write()的数据大小和缓冲区大小相同，磁盘文件默认采取这种方式进行缓冲。

```c
#include <stdio.h>

void setbuf(FILE *stream, char *buf);
```
- 等同于`setvbuf(fp, buf, (buf != NULL) ? _IOFBF: _IONBF, BUFSIZ)`

```c
#define _BSD_SOURCE
#include <stdio.h>

void setbuffer(FILE *stream, char *buf, size_t size);
```
- 等同于`setvbuf(fp, buf, (buf != NULL) ? _IOFBF : _IONBF, size)`

### Flushing a stdio buffer
```c
#include <stdio.h>

int fflush(FILE *stream);
```
- 当stream为输出流时，fflush()将强制将stdio输出流内的数据通过write() system call写入到kernel buffer中
- 当stream为输入流时，fflush()将丢弃所有输入流已经缓冲的数据

## Controlling Kernel Buffering of File I/O
- synchronized I/O completion: 一个I/O操作，要么数据已经成功的被传输到磁盘，要么被诊断为不成功的操作
> synchronized I/O completion to mean “an I/O operation that has either been successfully transferred [to the disk] or diagnosed as unsuccessful.”

### System calls for controlling kernel buffering of file I/O
```c
#include <unistd.h>

int fsync(int fd);
```

```c
#include <unistd.h>

int fdatasync(int fd);
```

```c
#include <unistd.h>

void sync(void);
```

## Summary of I/O Buffering
![13-1.png](img/13-1.png)