# FILE I/O: FURTHER DETAILS

## Atomicity and Race Conditions
- 使用open()方法打开并创建文件时，可以通过同时传入O_CREATE和O_EXCL标志来保证open()操作原子性
- 可以通过传入O_APPEND标志来保证数据写入操作和偏移量移动操作为同一原子操作

##  Open File Status Flags
- 可以通过fcntl()方法来获取和改变访问模式和状态标志

## File Descriptors and Open File  
![5-2.png](./img/5-2.png)

1. 进程级别的文件表述表
2. 系统级别的打开文件表
3. 文件系统的i-node表

- 同一进程内可以通过dup(),dup()2和fcntl()，实现同进程内两个不同的文件描述符(file descriptor)指向一个文件句柄(open file description)
- 不同进程可以通过fork()，实现不同进程内的file descriptor指向同一个open file description
- 不同进程内各自对同一文件调用open()或者同一进程内多次调用open(), 实现进程内不同的file descriptor指向不同的open file description但是指向同一i-node表中的条目

##  File I/O at a Specified Offset: pread() and pwrite()
pread()和pwrite()不会移动open file中的文件偏移量，相当于lseek，read/write、lseek操作同事纳入了同一原子性的操作

## Scatter-Gather I/O: readv() and writev()
- readv()系统调用实现了分散输入的功能，从文件表舒服fd所指向的文件中读取一片连续的字节，然后散落放置在iov指定的缓冲区里
- writev()系统调用实现了集中输出，将iov所指定的所有缓冲区里的数据拼接起来，然后以联系字节的形式写入到fd所指向的文件当中

## Truncating a File: truncate() and ftruncate()
- 若文件长度大于参数length，调动将丢弃超出部分；若是小于参数length，调用将在文件尾部添加一系列空字节或者文件空洞(file holes)

## Nonblocking I/O
- pipeline、socket和device都支持nonblocking mode，因为无法在open()函数传入O_NONBLOCK标志，所以必须要通过fcntl()的F_SETFL命令来设置

## I/O on Large Files
- 通过定义宏来打开LFS操作，必须在所有include头文件之前定义
```c
#define _FILE_OFFSET_BITS 64
```