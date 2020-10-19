# MEMORY ALLOCATION

## Allocating Memory on the Heap
- 堆是一段长度可变的连续虚拟内存，始于进程的未初始化的数据段末尾，随着内存的分配和释放而增减

## Adjusting the Program Break: brk() and sbrk()
```c
#include <unistd.h>

int brk(void *end_data_segment);
void *sbrk(intptr_t increment);
```
- brk()会讲program break设置为参数end_data_segment所指定的位置。由于虚拟内存以页为单位进行分配，end_data_segment会四舍五入到下一个内存页边界处
- sbrk()将program break在原有地址上增加从参数increment传入的大小，sbrk()返回前一个program break的地址

## Allocating Memory on the Heap: malloc() and free()
```c
#include <stdlib.h>

void* malloc(size_t size);
```
- malloc()的返回类型是void*，因为可以将其赋值给任意类型的C指针
- free()并不降低program break的位置，而是将这块内存增加到空闲内存列表中，供后续的malloc()函数循环使用