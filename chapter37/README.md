# DAEMONS

## Overview
- `daemon process` 存活时间很长，通常在系统启动时被创建直到运行系统被关。在后台运行且不拥有`controlling terminal`，这样可以确保kernel永远不会为`daemon process`生成job-control和terminal-related的signal(`SIGINT` `SIGTSTP` `SIGHUP`)

## Creating a Daemon
- 执行`fork()`，之后parent process退出，child process会变成`init` process的child process，child process继续执行
    - 假如`daemon`是通过命令行启动的，shell则会通知parent process已经终止，之后会显示出另一个shell的提示符并且让child process继续在后台运行
    - child process被确保不会成为process group的leader，因为process group的leader一旦退出，就不再要求有一个succeeding leader，因此此时的child process的PGID和PID是不同的，这样才能通过`setsid()`来开启一个新session
- child process调用`setsid()`来启动一个新session并且释放之前所有其关联的`controlling terminal`
    - 此时child process会成为新session的leader并且成为session内新process group的leader，也就是此时PID和PGID和SID是相等的
    - 此时的session是没有任何的controlling terminal，而且也会和之前的controlling terminal断连
- 如果此后child process不会打开任何的terminal device，则无需要担心child process会获得controlling terminal。如果有后续要打开terminal devices的需要，则需要采取一些措施来确保device不会成为controlling terminal
    - 对所有有可能应用到一个terminal device的`open()`调用添加`O_NOCTTY`标志，此时的daemon process是child process
    - 或者在`setsid()`之后执行第二次的`fork()`，此时child process会创建出grandchild process，然后让child process退出，这样确保grandchild process永远不会成为session的leader，因为不是session leader则不会请求到一个controlling terminal，此时的daemon process是grandchild process
- 使用`umask()`来清除process umask，以确保daemon process在创建文件和目录时候拥有所需的权限，
- 使用`chdir()`来修改daemon process的工作目录，通常会改为根目录`/`
- 使用`close()`来关闭所有daemon process从parent process继承的open file descriptors，因为file descriptor是一种有限的资源
    - 这一步不是必须的，取决于daemon process是否需要从parent process继承而来的open file descriptor
    - 执行这一步的主要原因是daemon作为一个在后台运行且失去controlling terminal的process，保留open fd`0` `1` `2`无意义，因为他们通常指向一个terminal，而且由于长时间保留着打开文件导致无法卸载这个被打开文件的文件系统
- 在关闭了open fd`0` `1` `2`之后，daemon process会打开`/dev/null`，并使用`dup2()`或者相似的函数来使得这些fd重新指向这个device
    - 保证daemon process在后续调用I/O库，且这些I/O库也会使用到fd`0` `1` `2`，时不会导致不可预期的错误
    - 防止daemon process在后续使用fd `1` `2`来打开文件的可能性，因为一个函数库的方法会使用这两个fd来处理stdout和stderr

## Guidelines for Writing Daemons
- 很多标准daemon会在系统关闭时通过特定脚本来终止
- 如果没有提供特定脚本，系统在关闭时，init会向所有的daemon发送一个`SIGTERM`signal，`SIGTERM`默认的disposition是终止一个process。如果daemon需要执行一些清理工作，那么需要为此signal建立一个handler，`init`在发完`SIGTERM`signal之后5秒会发送一个`SIGKILL`signal

## Using SIGHUP to Reinitialize a Daemon

## Logging Messages and Errors Using syslog

### Overview

### The syslog API

### The /etc/syslog.conf File