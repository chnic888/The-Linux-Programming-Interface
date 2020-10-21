# SYSTEM PROGRAMMING CONCEPTS

## System Calls
- system call是受控的内核入口，进程可以请求内核以自己的名义去执行某些动作  
- system call将处理器从用户态切换到核心态，以便CPU访问受到保护的内核内存
- 每个system call都有一个唯一的数字来标识
- 每个system call可以按照规定好的一套参数，对用户空间（亦即进程的虚拟地址空间）与内核空间之间（相互）传递信的信息加以规范
![3-1.png](./img/3-1.png)

> 1. 应用程序通过调用C语言函数中的wrapper函数，来发起system call
> 2. wrapper函数必须保证system call的所有参数对于trap-handling routine可用。这些参数通过stack传入wrapper函数，但是内核希望这些参数被放置于特定的寄存器中，因此wrapper function会将上述的参数拷贝到寄存器中
> 3. 由于所有system call进入内核的方式相同，所以内核需要能区分出每个系统调用。wrapper函数会将system call编号复制到一个特殊的CPU寄存器中(%eax)
> 4. wrapper函数执行一条trap machine instruction(int 0x80)，引发处理器从用户态切换到核心态，并执行system trap 0x80(十进制的128)所指向的代码
> 5. 为了响应0x80，kernel会调用system_call()（位于汇编文件arch/i386/enter.S中）来处理这次trap  
>   a) 在内核栈中保存寄存器值  
>   b) 审核system call number的有效性  
>   c) 通过system call number去一个记录所有system call service routine表(内核变量sys_call_table)中索引，找到对应的system call service routine并进行调用。如果system call service routine含有参数，则会优先验证参数的有效性  
>   d) 从内核栈中恢复各寄存器的值，并将系统调用返回值置于栈中  
>   e) 返回至wrapper函数，同时将处理器切换回用户态  
> 6. 若system call service routine返回值表用此次调用出错，wrapper函数会使用该值来设置全局变量errno。然后wrapper函数会返回到调用程序，并同时返回一个整数值，以表明系统调用是否成功