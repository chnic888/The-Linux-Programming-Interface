# CAPABILITIES

## Rationale for Capabilities
- 传统的UNIX privilege scheme将process分为两类
    - effective user ID为0的超级用户process，他会绕过所有特权检查
    - 根据process user和group IDs进行特权检查的其他process
- Linux capability scheme不再使用单一权限来在kernel中进行安全检查，superuser privilege被换分成了不同的单元，也被成为`capabilities`，每个privileged operation都对应着特定的`capabilities`，process只有在具有相应`capabilities`时才能执行相应的操作，不需再检查`effective user ID`

## The Linux Capabilities

## Process and File Capabilities
- 每个process都会有三个相关联的capability sets，`permitted` `effective`和`inheritable`

### Process Capabilities
- kernel会为每个process都维护`permitted` `effective`和`inheritable`这三个capability sets，每个set都可以启用0到多个capability
- `permitted` 一个process可以使用的capability的set，只有`permitted set`内的capability才可以被`effective set`和`inheritable set`的选择，把一个capability从process的`permitted set`是一个**不可逆**的行为，将导致process无法再重新获得改capability
- `effective` kernel会使用`effective set`内的capability来对process进行权限检查，把一个capability从process的`effective set`是一个**可逆**的行为，会临时禁用这个capability，但之后可以将其再度还原
- `inheritable` process在执行一个新程序时，可以将`inheritable set`内的capability带入到`permitted set`中
- 通过`fork()`产生的child process会继承parent process的capability sets的副本

### File Capabilities

### Purpose of the Process Permitted and Effective Capability Sets

### Purpose of the File Permitted and Effective Capability Sets

### Purpose of the Process and File Inheritable Sets

### Assigning and Viewing File Capabilities from the Shell

## The Modern Capabilities Implementation

## Transformation of Process Capabilities During exec()

### Capability Bounding Set

### Preserving root Semantics

## Effect on Process Capabilities of Changing User IDs

## Changing Process Capabilities Programmatically

## Creating Capabilities-Only Environments

## Discovering the Capabilities Required by a Program

## Older Kernels and Systems Without File Capabilities