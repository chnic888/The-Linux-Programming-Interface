# THREADS: FURTHER DETAILS

## Thread Stacks

## Threads and Signals

### How the UNIX Signal Model Maps to Threads

### Manipulating the Thread Signal Mask
```c
#include <signal.h>

int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset);
```

### Sending a Signal to a Thread
```c
#include <signal.h>

int pthread_kill(pthread_t thread, int sig);
```

```c
#define _GNU_SOURCE
#include <signal.h>

int pthread_sigqueue(pthread_t thread, int sig, const union sigval value);
```

### Dealing with Asynchronous Signals Sanely
```c
#include <signal.h>

int sigwait(const sigset_t *set, int *sig);
```

## Threads and Process Control

## Thread Implementation Models

### Many-to-one (M:1) implementations (user-level threads)

### One-to-one (1:1) implementations (kernel-level threads)

### Many-to-many (M:N) implementations (two-level model)

## Linux Implementations of POSIX Threads

### LinuxThreads

### NPTL

### Which Threading Implementation?

## Advanced Features of the Pthreads API

