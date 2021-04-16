# LOGIN ACCOUNTING

## Overview of the utmp and wtmp Files

## The utmpx API

## The utmpx Structure

## Retrieving Information from the utmp and wtmp Files
```c
#include <utmpx.h>

void setutxent(void);
```

```c
#include <utmpx.h>

void endutxent(void);
```

```c
#include <utmpx.h>

struct utmpx *getutxent(void);
struct utmpx *getutxid(const struct utmpx *ut);
struct utmpx *getutxline(const struct utmpx *ut);
```

```c
#define _GNU_SOURCE
#include <utmpx.h>

int utmpxname(const char *file);
```

## Retrieving the Login Name: getlogin()
```c
#include <unistd.h>

char *getlogin(void);
```

## Updating the utmp and wtmp Files for a Login Session
```c
#include <utmpx.h>

struct utmpx *pututxline(const struct utmpx *ut);
```

```c
#define _GNU_SOURCE
#include <utmpx.h>

void updwtmpx(char *wtmpx_file, struct utmpx *ut);
```

## The lastlog