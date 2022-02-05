# MEMORY MAPPINGS

## Overview

- `mmap()`syscall在calling process的虚拟地址空间中创建一个新的内存mapping
	- `file mapping` 将文件的一个区域直接mapping到calling
	  process的虚拟内存中。一旦一个文件被mapping，其内容就可以通过对相应内存区域中的字节的操作来访问，mapping页会按需的从文件中自动加载
	- `anonymous mapping` 不会有具体相对应的文件，则会mapping一个内容被初始化为0的虚拟文件
- 一个process mapping的内存可以与其他process的mapping共享，即每个process的页表项指向相同的RAM页
	- 如果两个process mapping文件的同一区域，则它们会共享相同的物理内存页
	- 由`fork()`创建的child process会继承parent process mapping的副本，child中的mapping与parent中的相应mapping会引用相同的物理内存页
- 当两个或多个process共享相同的页时，每个process都可能看到其他process对页内容所做的更改，具体取决于mapping是私有的还是共享
	- `private mapping (MAP_PRIVATE)` mapping的内容中发生的修改对其他process不可见，对于文件mapping来说，变更不会发生在底层文件上
	- `shared mapping (MAP_SHARED)` mapping的内容中发生的修改对所有共享同一个mapping的process都可见，对于文件mapping来说，变更会发生在底层的文件上

| Visibility of modifications | File mapping                                               | Anonymous mapping                      |
|-----------------------------|------------------------------------------------------------|----------------------------------------|
| Private                     | Initializing memory from contents of file                  | Memory allocation                      |
| Shared                      | Memory-mapped I/O; sharing memory between processes (IPC)  | Sharing memory between processes (IPC) |

- `private file mapping` mapping的内容会用文件区域中的内容来初始化
- `private anonymous mapping` 每次调用`mmap()`都会产生一个新mapping，且该mapping与同一或不同process创建的匿名mapping不同，即不会共享物理页
- `shared file mapping` mapping文件同一区域的所有process会共享相同的内存物理页，且物理页是用文件区域的内容来初始化的
- `shared anonymous mapping` 每次调用`mmap()`都会产生一个新的和不同的mapping，且该mapping不与任何其他mapping共享页面

- 当process执行`exec()`时，mapping会丢失，但通过`fork()`创建的child process则会继承mapping，同时mapping的类型`MAP_PRIVATE`或`MAP_SHARED`也会被继承

## Creating a Mapping: mmap()

```c
#include <sys/mman.h>

void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
```

- `mmap()`syscall会在calling process的虚拟地址空间中创建一个新的mapping
- `addr`
- `length`
- `prot`
- `flags`
- `fd`
- `offset`

| Value      | Description                                |
|------------|--------------------------------------------|
| PROT_NONE  | The region may not be accessed             |
| PROT_READ  | The contents of the region can be read     |
| PROT_WRITE | The contents of the region can be modified |
| PROT_EXEC  | The contents of the region can be executed |

## Unmapping a Mapped Region: munmap()

```c
#include <sys/mman.h>

int munmap(void *addr, size_t length);
```

## File Mappings

### Private File Mappings

### Shared File Mappings

### Boundary Cases

### Memory Protection and File Access Mode Interactions

## Synchronizing a Mapped Region: msync()

```c
#include <sys/mman.h>

int msync(void *addr, size_t length, int flags);
```

## Additional mmap() Flags

## Anonymous Mappings

## Remapping a Mapped Region: mremap()

```c
#define _GNU_SOURCE
#include <sys/mman.h>

void *mremap(void *old_address, size_t old_size, size_t new_size, int flags, ...);
```

## MAP_NORESERVE and Swap Space Overcommitting

## The MAP_FIXED Flag

## Nonlinear Mappings: remap_file_pages()

```c
#define _GNU_SOURCE
#include <sys/mman.h>

int remap_file_pages(void *addr, size_t size, int prot, size_t pgoff, int flags);
```
