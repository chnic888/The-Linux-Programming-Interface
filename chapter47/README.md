# SYSTEM V SEMAPHORES
- System V的信号量不是用来process之间传输数据的，而是用来同步process之间的动作
- 信号量是由kernel维护的整数，其值被限制为大于或等于0，在一个信号量上可以执行如下的操作，且最后两个操作会导致calling process被阻塞
  - 将信号量设置成为一个绝对值
  - 给信号量当前的值加上一个数值
  - 给信号量当前的值减去一个数值
  - 等待一个信号量的值等于0，

- 当减少一个信号量的值时，kernel会把所有试图将信号量值减少到0之下的操作阻塞
- 如果一个信号量的值当前不等于0, 那么一个正在等待信号量的值等于0的calling process将会一直被kernel阻塞

![47-1.png](./img/47-1.png)

## Overview

## Creating or Opening a Semaphore Set

## Semaphore Control Operations

## Semaphore Associated Data Structure

## Semaphore Initialization

## Semaphore Operations

## Handling of Multiple Blocked Semaphore Operations

## Semaphore Undo Values

## Implementing a Binary Semaphores Protocol

## Semaphore Limits

## Disadvantages of System V Semaphores