# VIRTUAL MEMORY OPERATIONS

## Changing Memory Protection: mprotect()

```c
#include <sys/mman.h>

int mprotect(void *addr, size_t length, int prot);
```

## Memory Locking: mlock() and mlockall()

## Determining Memory Residence: mincore()

## Advising Future Memory Usage Patterns: madvise()

```c
#define _BSD_SOURCE
#include <sys/mman.h>

int madvise(void *addr, size_t length, int advice);
```