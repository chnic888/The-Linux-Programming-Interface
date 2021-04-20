# CAPABILITIES

## Rationale for Capabilities
- 传统的UNIX privilege scheme将process分为两类
    - effective user ID为0的超级用户process，他会绕过所有特权检查
    - 根据process user和group IDs进行特权检查的其他process
- Linux capability scheme不再使用单一权限来在kernel中进行安全检查，superuser privilege被换分成了不同的单元，也被成为`capabilities`，每个capability都可以被单独的启用和禁用
- Linux capability是被划分到thread级别的属性

## The Linux Capabilities

## Process and File Capabilities
- 每个process都会有三个相关联的capability sets，`permitted` `effective`和`inheritable`，由于capabilities是tread级别属性，所以实际上每个thread也包含这三个set

### Process Capabilities
- kernel会为每个process都维护`permitted` `effective`和`inheritable`这三个capability sets，每个set都可以启用0到多个capability
- `permitted` 一个process的`effective set`和`inheritable set`上限，把一个capability从process的`permitted set`是一个**不可逆**的行为，将导致process无法再重新获得改capability
- `effective` kernel会使用`effective set`内的capabilities来对process进行权限检查，把一个capability从process的`effective set`是一个**可逆**的行为，会临时禁用这个capability，但之后可以将其再度还原
- `inheritable` process在执行一个新程序时，决定哪些capabilities可以带入到新程序的`permitted set`中
- 通过`fork()`产生的child process会继承parent process的capability sets的副本

### File Capabilities
- 如果一个可执行文件有关联的`capability sets`，这些set会结合着process的`capability sets`来决定执行了`execve(2)`之后的thread的capabilities
- `permitted` process在通过`exec()`来执行这个文件时，set包含的capabilities会自动添加到process的`permitted set`中
- `effective` 单个的bit位，如果被启用，在执行`exec()`时，process的新的启用的`permitted set`会在`effective sets`中启用。如果被禁用，在对文件执行了`exec()`之后，process的新的`effective set`将会初始化为空
- `inheritable` 与process的`inheritable set`取mask，来确定process在执行`exec()`之后那些capabilities在`permitted set`可以被启用

### Purpose of the Process Permitted and Effective Capability Sets
- `process permitted capability set` 定义了一个process的`effective set`的上限，只有在`permitted set`中的capability才能被添加进`effective se`t中
- `process permitted capability set` 定义了一个process里可以被使用的capabilities
- `process effective capability set` 定义了process当前生效的capabilities，kernel会使用这些capabilities来检查process是否有足够的权限来执行特定的操作

### Purpose of the File Permitted and Effective Capability Sets
- `file permitted capability set` 提供了可执行文件可以向process赋予capabilities机制，可以指定可执行文件的哪些capabilities可以在`exec()`被执行之后被赋予process的capabilities
- `file effective capability set` 是一个单个的bit标志位，决定了process在对文件执行了`exec()`之后，`effective sets`中会启用哪些capabilities

### Purpose of the Process and File Inheritable Sets
- process的`permitted capabilities`在通过一个`exec()`调用之后是不会被保存的，因此需要引入`inheritable set`来解决这个问题
- `inheritable set` 提供了一套机制使得process在调用`exec()`之后可以保留其一部分的capabilities

### Assigning and Viewing File Capabilities from the Shell

## The Modern Capabilities Implementation
- 对于需要特权的操作，kernel需要检查process是否有相应的capability，而不是检查`effective user ID`是否为`0`
- kernel必须提供允许获取一个process的capabilities和修改的system calls
- kernel必须支持将capabilities附加到文件的机制，这样文件被执行时process就可以获取到相应的capabilities

## Transformation of Process Capabilities During exec()
- 在执行`exec()`期间，kernel会根据如下的规则来计算process的新的capabilities
```
P'(permitted) = (P(inheritable) & F(inheritable)) | (F(permitted) & cap_bset)

P'(effective) = F(effective) ? P'(permitted) : 0

P'(inheritable) = P(inheritable)
```
- `P`表示调用`exec()`之前的capabilities
- `P'`表示调用`exec()`之后的capabilities
- `F`表示文件的capabilities
- `cap_bset`表示capability边界集合的值
- `exec()`不会改变process的`inheritable set`

### Capability Bounding Set
- `capability bounding set`是一个process级别的属性，并且会在`fork()`被child process所继承

### Preserving root Semantics
- 如果在执行一个` set-user-ID`的程序，permitted和effective的capabilities计算就会被简化成如下
```
P'(permitted) = P(inheritable) | cap_bset

P'(effective) = P'(permitted)
```

## Effect on Process Capabilities of Changing User IDs

## Changing Process Capabilities Programmatically

## Creating Capabilities-Only Environments

## Discovering the Capabilities Required by a Program

## Older Kernels and Systems Without File Capabilities