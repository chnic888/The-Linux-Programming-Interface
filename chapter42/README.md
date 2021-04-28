# ADVANCED FEATURES OF SHARED LIBRARIES

## Dynamically Loaded Libraries

### Opening a Shared Library: dlopen()
```c
#include <dlfcn.h>

void *dlopen(const char *libfilename, int flags);
```

### Diagnosing Errors: dlerror()
```c
#include <dlfcn.h>

const char *dlerror(void);
```

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