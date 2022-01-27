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

- `dlsym()`在`handle`指向的库以及该库的依赖树中搜索名为`symbol`的symbol(函数或者变量) address，如果找不到则返回`NULL`
- `dlsym()`的返回的`symbol value`有可能为NULL，这一点与`symbol not found`的返回值则无法区分，为了能区分出不同的情况，必须先调用`dlerror()`

- 如果symbol是一个变量的名称

```c
int *ip;

ip = (int *) dlsym(symbol, "myvar");
if (ip != NULL)
    printf("Value is %d\n", *ip);
```

- 如果symbol是一个函数的名称

```c
int (*funcp)(int);

/* C99标准禁止函数指针和void *之间的赋值操作，因此无法使用 funcp = dlsym(handle, symbol);*/

*(void **) (&funcp) = dlsym(handle, symbol);
// 或
funcp = (int (*) (int)) dlsym(handle, symbol);
```

### Closing a Shared Library: dlclose()

```c
#include <dlfcn.h>

int dlclose(void *handle);
```

- `dlclose()`会减小handle引用的库的计数，如果计数变为0且其他库也不需要该库的symbol了，则会卸载这个库

### Obtaining Information About Loaded Symbols: dladdr()

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

- `dladdr()`返回一个给定地址`addr`对应的`Dl_info`结构的信息，`addr`通常是通过`dlsym()`来获得

### Accessing Symbols in the Main Program

- 如果主程序使用`dlopen()`加载一个共享库A，并且使用`dlsym()`获取到了`x()`的地址，`x()`中又调用了`y()`，`y()`也许在A的依赖树中可以找到，此时需要`x()`回调主程序中的`y()`
  ，则需要提供`––export–dynamic`链接选项

```c
$ gcc -Wl,--export-dynamic main.c

//等价命令
$ gcc -export-dynamic main.c
```

## Controlling Symbol Visibility

- 使用C语言提供的`static`关键字，使得一个symbol私有于一个源代码模块，从而使得他无法被其他目标文件绑定
- 使用gcc提供的一个专有的编译属性，执行了和`static`关键字类似的任务
  ```c
  void __attribute__ ((visibility("hidden")))
  func(void) {
    /* Code */
  }	
  ```
- 使用version scripts来精确的控制symbol的可见性
- 动态加载一个共享库时，为`dlopen()`指定`RTLD_GLOBAL`标志位

## Linker Version Scripts

- `version script` 是一个包含连接器执行指令的文本文件，使用`version script`必须指定`––version–script`链接选项

```shell
$ gcc -Wl,--version-script,myscriptfile.map ...
```

### Controlling Symbol Visibility with Version Scripts

### Symbol Versioning

## Initialization and Finalization Functions

- 不管是被自动加载还是显式的使用`dlopen()`加载，初始化函数和终止函数都会被自动执行

```c
void __attribute__ ((constructor)) some_name_load(void)
{
 /* Initialization code */
}
```

```c
void __attribute__ ((destructor)) some_name_unload(void)
{
 /* Finalization code */
}
```

## Preloading Shared Libraries

## Monitoring the Dynamic Linker: LD_DEBUG