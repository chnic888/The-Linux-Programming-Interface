# SIGNALS: FUNDAMENTAL CONCEPTS

## Concepts and Overview
- 信号是事件发生时对进程的一种通知机制
- 一个有合适权限的进程可以向另外一个进程发送信号 
- 进程也可以向自身发送信号
- kernel也可以向进程发送信号，大多数进程收到的信号来源都是kernel
    - 硬件发生异常
    - 用户键入了能够产生信号的终端特殊字符(Control-C or Control-D)
    - 发生了软件事件
- 信号通过某些事件来产生。一旦信号被产生，会稍后被传递给进程，
