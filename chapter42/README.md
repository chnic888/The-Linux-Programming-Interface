# ADVANCED FEATURES OF SHARED LIBRARIES

## Dynamically Loaded Libraries
- 在Linux上使用`dlopen`API来构建程序，则必须指定`–ldl`选项以便与`libdl`库链接起来

### Opening a Shared Library: dlopen()
```c
#include <dlfcn.h>

void *dlopen(const char *libfilename, int flags);
```
- `dlopen()`将名字为`libfilename`的共享库加载进calling process的虚拟地址空间，并且增加这个库的打开引用计数
- `dlopen()`成功调用后会返回共享库的句柄以供后续的`dlopen API`来使用
- 如果`libfilename`指定的共享库包含依赖于其他共享库，则`dlopen()`将会自动加载依赖共享库
- 可以对同一个共享库进行多次的`dlopen()`调用，但是库只会在第一次调用时被加载进内存一次，之后的调用不会进行内存加载且会返回与第一次相同的handle值。
- `dlopen API`为每个库的的handle维护一个计数，每次调用`dlopen()`会增加计数，调用`dlclose()`会减少计数，并且只有在计数为0时`dlclose()`才会从内存中卸载这个共享库

### Diagnosing Errors: dlerror()
```c
#include <dlfcn.h>

const char *dlerror(void);
```
- `dlerror()` 如果在调用`dlopen()`或者`dlopen API`的其他函数时得到了一个错误，调用`dlerror()`可以获取一个指向表明错误原因的字符串指针

### Obtaining the Address of a Symbol: dlsym()
```c
#include <dlfcn.h>

void *dlsym(void *handle, char *symbol);
```

### Closing a Shared Library: dlclose()
```c
#include <dlfcn.h>

int dlclose(void *handle);
```

###  Obtaining Information About Loaded Symbols: dladdr()
```c
#define _GNU_SOURCE
#include <dlfcn.h>

int dladdr(const void *addr, Dl_info *info);

typedef struct {
    const char *dli_fname; /* Pathname of shared library containing 'addr' */
    void *dli_fbase; /* Base address at which shared library is loaded */
    const char *dli_sname; /* Name of nearest run-time symbol with an address <= 'addr' */
    void *dli_saddr; /* Actual value of the symbol returned in 'dli_sname' */
} Dl_info;
```

### Accessing Symbols in the Main Program

## Controlling Symbol Visibility

## Linker Version Scripts

### Controlling Symbol Visibility with Version Scripts

### Symbol Versioning

## Initialization and Finalization Functions

## Preloading Shared Libraries

## Monitoring the Dynamic Linker: LD_DEBUG