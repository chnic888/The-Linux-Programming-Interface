# FUNDAMENTALS OF SHARED LIBRARIES

## Object Libraries
- 链接实际上是由一个单独的链接程序`ld`完成的。当使用`cc`或者`gcc`来链接一个程序时，编译器会在幕后调用`ld`链接器

## Static Libraries
- `static libraries`静态库也被称为`archives`，一个静态库实际上就是一个保存了所有被添加的object files副本的一个文件
- 静态库的默认命名规范为lib*name*.a
- 一个静态库的创建和维护通过`ar(1)`命令来完成，命令形式为
```shell
$ ar options archive object-file...
```
&nbsp;
- 程序和静态库链接起来的两种方式
    - 把静态库的全名作为链接命令的一部分
    ```shell
    $ gcc -c prog.c
    $ gcc -o prog prog.o libdemo.a
    ```
    - 将静态库放在链接程序可以搜索到的标准目录中，比如`/usr/lib`，然后使用`–l`选项来指定静态库名，也就是`libname.a`里**name**的部分
    ```shell
    $ gcc -o prog prog.o -ldemo
    ```
    - 如果静态库文件不在标准目录内，可以使用`-L`选项来指定静态库文件所处在的目录
    ```shell
    $ gcc -o prog prog.o -Lmylibdir -ldemo
    ```

## Overview of Shared Libraries

## Creating and Using Shared Libraries—A First Pass

### Creating a Shared Library

### Position-Independent Code

### Using a Shared Library

### The Shared Library Soname

## Useful Tools for Working with Shared Libraries

## Shared Library Versions and Naming Conventions

## Installing Shared Libraries

## Compatible Versus Incompatible Libraries

## Upgrading Shared Libraries

## Specifying Library Search Directories in an Object File

## Finding Shared Libraries at Run Time

## Run-Time Symbol Resolution

## Using a Static Library Instead of a Shared Library