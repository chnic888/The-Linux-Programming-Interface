# PROCESSES

## Processes and Programs
- 进程是由内核定义的抽象的实体，该实体负责分配程序运行的各项系统资源，从内核角度看，进程是用内存空间和一系列内核数据结构组成，其中用户内存空间包含了程序代码以及代码所使用的变量，而内核数据结构则用于维护进程状态信息

## Process ID and Parent Process ID
- pid_t getpid(void);  获取当前process id
- pid_t getppid(void);  获取当前parent process id

## Memory Layout of a Process
- Text Segment: 文本段包含了进程运行的程序机器语言指令
- Initialized Data Segment: 初始化数据段包含显示初始化的全局变量和静态变量
- Uninitialized Data Segment: 未初始化数据段包含了未进行显示初始化的全局变量和静态变量
- Stack: 一个动态增长的收缩的段，由栈帧组成
- Heap: 在运行时(为变量)动态进行内存分配的一块区域

![6-1.png](./img/6-1.png)

## Virtual Memory Management
- Spatial locality: 空间局部性是指程序倾向于访问最近访问过的内存地址附近的内存，由于指令是顺序执行的，且有时会按顺序处理数据结构
- Temporal locality: 是指程序倾向于在不久的将来再次访问最近刚方位过的内存地址，由于循环
- 虚拟内存的规划之一是将每个程序使用的内存切割成小型、固定大小的"页"单元，相应的也会将物理RAM划分成为与虚拟页尺寸大小的页帧
- 内核需要为每个进程维护一张页表，页表中每个条目要么指出一个虚拟页面在RAM中的所在位置，要么标明其当前驻留在磁盘上
![6-2.png](./img/6-2.png)

虚拟内存的优点
- 进程与进程、进程与内核相互隔离，所以一个进程不能读取或修改另一个进程或内核的内存
- 适当情况下，两个或者多个进程可以共享内存
- 便于实现内存保护机制
- 程序员和编译器、连接器之类的工具无需关注程序在RAM中的屋里布局
- 因为需要驻留在内存中的仅是程序的一部分，所以程序的加载和运行都很快

## Environment List
- 每一个进程都有一个与其相关称为环境列表的字符串数组
- 新进程在创建之时，会继承其父进程的环境列表
- 子进程只有在创建的时候才能获得其父进程的环境列表副本，所以种传递是单向且一次性的
```c
char *getenv(const char *name);

//a=b
int putenv(char *string);

int setenv(const char *name, const char *value, int overwrite);

int unsetenv(const char *name);

int clearenv(void);
```