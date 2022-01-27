# WRITING SECURE PRIVILEGED PROGRAMS

## Is a Set-User-ID or Set-Group-ID Program Required?

- 尽可能的避免编写`set-user-ID`和`set-group-ID`程序

## Operate with Least Privilege

- 一个`set-user-ID`或者`set-group-ID`程序通常只在执行特定操作时才需要权限，因此程序在执行其他工作时候就应该禁用这些权限
- 如果一个权限在之后的工作中再也不需要，那么经应该永久删除这些权限，程序总是应该使用`least privilege`来完成当前的任务，`saved set-user-ID`机制就是为了`least privilege`而设计的

### Hold privileges only while they are required

```c
uid_t orig_euid;
orig_euid = geteuid();
if (seteuid(getuid()) == -1)    /* Drop privileges */
    errExit("seteuid");

/* Do unprivileged work */

if (seteuid(orig_euid) == -1)   /* Reacquire privileges */
    errExit("seteuid");

/* Do privileged work */
```

### Drop privileges permanently when they will never again be required

- 永久的删除权限是通过将process的所有的`user (group) IDs`重置为和`real (group) ID`一样的值来实现的

```c
if (setuid(getuid()) == -1)
    errExit("setuid");
```

- 一个`set-user-ID-root`程序的`effective user ID`为`0`时，可以通过`setuid(getuid())`来重置所有的`user IDs`
- 当calling process的`effective user ID`不为`0`时，调用`setuid(getuid())`无法重置`saved set-user-ID`，此时`setuid()`
  只会修改`effective user ID`

```c
if (setreuid(getuid(), getuid()) == -1)
    errExit("setreuid");
```

- 一个`set-user-ID`程序的`effective user ID`不为`0`时，可以通过`setreuid()`或`setresuid()`来永久删除权限标识符

## Be Careful When Executing a Program

### Drop privileges permanently before execing another

- 如果一个`set-user-ID`或`set-group-ID`程序执行了另外一个程序，则需要确保process所有的`user (group) IDs`被重置为和`real user (group) ID`
  一样的值，这样新程序在启动时候就不会拥有权限并且也无法重新请求
- 可以在执行`exec()`之前执行`setuid(getuid())`来实现

### Avoid executing a shell (or other interpreter) with privileges

- 用户控制下的privileged program永远都不应该exec一个shell

### Close all unnecessary file descriptors before an exec()

- 在调用exec()之前应该关闭特权程序才能打开的资源文件，可以通过`close()`显示的关闭fd，也可以通过设置`close-on-exec`标志位来实现

## Avoid Exposing Sensitive Information

- 一个安全的程序应该避免产生core dump文件，可以使用`setrlimit()`将`RLIMIT_CORE`设置为0来防止产生core dump

## Confine the Process

### Consider using capabilities

- 一个process可以独立的启用和禁用`capabilities`单元，从而达到在拥有少量权限而无需使用root权限的情况下运行

### Consider using a chroot jail

- 使用`chdir()`来建立一个chroot jail来限制程序可访问的目录和文件

## Beware of Signals and Race Conditions

- 用户可以向他启动的`set-user-ID`program发送任意signal，其发送时间和发送频率也是任意的，需要考虑如果在程序执行的任何时候传递`signal`时可能发生的竞争条件
- 一个privileged process应当避免在一个已经不在成立的race condition上执行操作

## Pitfalls When Performing File Operations and File I/O

## Don’t Trust Inputs or the Environment

### Don’t trust the environment list

- `set-user-ID`和`set-group-ID`不应该建设环境变量的值是可靠的，尤其是`PATH`和`IFS`

### Handle untrusted user inputs defensively

- 一个privileged program应该仔细的验证的来自不可靠源的输入，并且进行验证

### Avoid unreliable assumptions about the process’s run-time environment

- 一个set-user-ID program应该避免其初始运行环境是可靠的，比如stdin、stdout和stderr有可能会被关闭

## Beware of Buffer Overruns

- 永远不要使用`gets()`，在使用`scanf()` `sprintf()` `strcpy()` `strcat()`时也需要预防缓冲区溢出

## Beware of Denial-of-Service Attacks

## Check Return Statuses and Fail Safely

- 一个privileged program应当总是检查system call或者library function是否成功以及是否返回了期望值