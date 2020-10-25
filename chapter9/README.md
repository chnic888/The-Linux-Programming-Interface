# PROCESS CREDENTIALS

## Real User ID and Real Group ID
- Real User ID and Real Group ID确定了process所属的用户和组
- login shell在登录过程中从/etc/passwd读取第三和第四字段来设置Real User ID and Real Group ID

## Effective User ID and Effective Group ID
- 当process试图执行一些操作的时候, 会结合Effective User ID and Effective Group ID加supplementary group IDs来确定process被授予什么样的权限
- The effective user and group IDs通常情况下和real IDs相等. system call和执行set-user-ID and set-group-ID可以使二者不相等

## Set-User-ID and Set-Group-ID Programs
- Set-user-ID program会将用户的Effective User ID置为可执行文件的拥有者的user ID
- Set-Group-ID program会将用户的Effective Group ID置为可执行文件的拥有者的group ID
```c
//Set-user-ID program
Effective User ID = user ID (owner) of the executable file

//Set-Group-ID program
Effective Group ID = group ID (owner) of the executable file
```

## Saved Set-User-ID and Saved Set-Group-ID


## File-System User ID and File-System Group ID


## Supplementary Group IDs        