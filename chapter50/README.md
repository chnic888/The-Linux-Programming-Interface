# VIRTUAL MEMORY OPERATIONS

## Changing Memory Protection: mprotect()

```c
#include <sys/mman.h>

int mprotect(void *addr, size_t length, int prot);
```

- `mprotect()`syscall更改从`addr`位置开始长度为`length`个字节范围内的虚拟内存分页的保护
- `addr`的取值必须是系统分页`sysconf(_SC_PAGESIZE)`大小的整数倍，且`addr`必须是分页对齐的
- `prot`是一个bit mask，用来指定这块内存上的新的保护，其取值必须是`PROT_NONE`或者是`PROT_READ` `PROT_WRITE`和`PROT_EXEC`一个或多个取OR
- 如果process试图以违反内存保护的方式访问内存区域，kernel会为该process发送一个`SIGSEGV`signal

## Memory Locking: mlock() and mlockall()

- `mlock()`和`mlockall()`可以将一个process的虚拟内存的部分或者全部锁进内存，以确保他们总是位于物理内存中
	- 可以提高性能，保证对锁定分页的访问不会因分页面错误而延迟，这对于必须确保快速响应时间的应用程序很有用
	- 可以保证安全，如果包含敏感数据的虚拟内存分页从未被换出，则不会将该分页的副本写入磁盘，也就无法从磁盘读出改内存分页的数据

### The RLIMIT_MEMLOCK resource limit

- 一个privileged process能够锁住的内存数量是没有限制的，即`RLIMIT_MEMLOCK`会被忽略
- 一个unprivileged process能够锁住内存的数量是由`RLIMIT_MEMLOCK`定义的软限制来决定的

- soft和hard的`RLIMIT_MEMLOCK`限制默认情况下都是8个pages，影响的范围有
	- `mlock()`和`mlockall()`
	- 使用`MAP_LOCKED`标记的`mmap()`
	- `shmctl()`的`SHM_LOCK`操作

### Locking and unlocking memory regions

```c
#include <sys/mman.h>

int mlock(void *addr, size_t length);
int munlock(void *addr, size_t length);
```

- 一个process可以使用`mlock()`和`munlock()`来给一块内存区域加锁和解锁
- `mlock()`syscall会锁住calling process的虚拟地址空间中的起始位置为`addr`长度为`length`字节区域中的所有分页，`addr`无需分页对齐，kernel会从`addr`
  下面的下一个分页边界开始锁住page
- 加锁操作的单位是分页，因此被锁住的区域的结束位置是大于`addr+length`的下一个分页的边界

```c
mlock(2000, 4000) /* page size 4096 bytes,  will lock bytes 0 through to 8191 */
```

- 除了`munlock()`之外，内存锁在下面几种情况下也会被自动删除
	- process终止时
	- 当被锁住的分页通过`munmap()`解除mapping时
	- 当被锁住的分页被使用`mmap()`的`MAP_FIXED`标记覆盖mapping时

### Details of the semantics of memory locking

- 通过`fork()`创建的child process不会继承内存锁，也不会在`exec()`中保留
- 在多个process共享一组分页的情况下，比如通过`MAP_SHARED`mapping，只要至少还有一个process在分页上持有内存锁，这些分页就会保持被锁定在内存中
- 内存锁不会在单个process上叠加，如果一个process重复的在一个特定的虚拟地址上调用`mlock()`，那么只会建立一个锁，并且只需要通过一个`munlock()`就删除这个锁
- 如果使用`mmap()`将同一组页面（即同一个文件）mapping到单个process中的几个不同位置，然后锁定这些mapping中的每一个，则这些页面保持锁定在`RAM`中，直到所有mapping都被解锁

### Locking and unlocking all of a process’s memory

```c
#include <sys/mman.h>

int mlockall(int flags);
int munlockall(void);
```

- 一个process可以使用`mlockall()`和`munlockall()`锁定和解锁其所有内存
- `flags`是一个bit mask，用来控制一个process虚拟地址空间中当前所有mapping锁定的行为，可以一个或者多个取OR
	- `MCL_CURRENT`将calling process虚拟地址空间中当前所有mapping的分页锁进内存，包括当前`program text` `data segment` `memory mappings`和`stack`
	- `MCL_FUTURE` 将会把所有之后mapping到calling process虚拟地址空间的分页锁定进内存

## Determining Memory Residence: mincore()

```c
#define _BSD_SOURCE /* Or: #define _SVID_SOURCE */
#include <sys/mman.h>

int mincore(void *addr, size_t length, unsigned char *vec);
```

- `mincore()`syscall是`mlock`的补充，它报告一个虚拟地址范围内的哪些页面当前驻留在`RAM`中，因此在访问这些分页时候也不会导致页面错误
- `mincore()`syscall返回起始地址为`addr`长度为`length`字节的虚拟地址范围内分页的内存驻留信息
    - `addr`指定的地址必须是内存对齐的
    - `length`实际会被向上舍入到系统分页大小的下一个整数倍，因为`mincore()`返回的信息是有关整个分页的
- `vec` 内存驻留的信息会通过`vec`来返回，`vec`为一个数组，大小为`(length + PAGE_SIZE – 1) / PAGE_SIZE`字节

## Advising Future Memory Usage Patterns: madvise()

```c
#define _BSD_SOURCE
#include <sys/mman.h>

int madvise(void *addr, size_t length, int advice);
```