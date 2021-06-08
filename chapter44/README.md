# PIPES AND FIFOS

## Overview

### A pipe is a byte stream
- 在使用pipe时候是不存在消息或者消息边界概念的，从pipe中读取数据的process可以读取任意大小的数据块，不用关心写入process的数据块的大小
- 通过pipe传递的数据是顺序的，从pipe中读取出来的字节顺序和他们被写入pipe的顺序是完全一致的，因此如法在pipe中使用`lseek()`来随机访问数据

### Reading from a pipe
- 尝试从一个当前为空的pipe中读取数据将会被阻塞，直至至少有一个字节被写入到pipe中为止
- 如果pipe的写入端被关闭了，消费端的process将会在读取完pipe剩余数据之后得到文件的结尾

### Pipes are unidirectional
- pipe中的数据传输是单向的，一段用于写入，另外一段则用于读取

### Writes of up to PIPE_BUF bytes are guaranteed to be atomic
- 如果多个process同时写入一个pipe，在同一时刻写入的数据量不超过`PIPE_BUF`字节，则可以确保写入的数据不会发生混合的情况

### Pipes have a limited capacity

## Creating and Using Pipes
```c
#include <unistd.h>

int pipe(int filedes[2]);
```

## Pipes as a Method of Process Synchronization

## Using Pipes to Connect Filters

## Talking to a Shell Command via a Pipe: popen()
```c
#include <stdio.h>

FILE *popen(const char *command, const char *mode);
int pclose(FILE *stream);
```

## Pipes and stdio Buffering

## FIFOs
```c
#include <sys/stat.h>

int mkfifo(const char *pathname, mode_t mode);
```

## A Client-Server Application Using FIFOs

## Nonblocking I/O

## Semantics of read() and write() on Pipes and FIFOs