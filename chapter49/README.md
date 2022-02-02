# MEMORY MAPPINGS

## Overview

## Creating a Mapping: mmap()

## Unmapping a Mapped Region: munmap()

## File Mappings

### Private File Mappings

### Shared File Mappings

### Boundary Cases

### Memory Protection and File Access Mode Interactions

## Synchronizing a Mapped Region: msync()

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
