# ACCESS CONTROL LISTS

## Overview
每条ACL记录都由三部分组成
- `type` 表示该记录是一个用户、组还是其他类别的用户
- `qualifier` 这个标志位是可选的，用来标识特定的用户或者组
- `permission set` 具体的授权信息

### ACL entries
- `ACL_MASK` 指定了ACL_USER、ACL_GROUP_OBJ和ACL_GROUP类型的ACE可授予的最大权限，一个ACL最多只能包含一个ACL_MASK的ACE。假如ACL包含ACL_USER或者ACL_GROUP的ACE，则必须包含ACL_MASK

### Minimal and extended ACLs
- `minimal ACL` 语义上等同于传统的文件权限集合， 由ACL_USER_OBJ、ACL_GROUP_OBJ和ACL_OTHER组成
- `extended ACL` 除了包含minimal ACL之外，还包含ACL_USER、ACL_GROUP和ACL_MASK

## ACL Permission-Checking Algorithm

## Long and Short Text Forms for ACLs
| Tag text forms | Tag qualifier present? | Corresponding tag type | Entry for |
| --- | --- | --- | --- |
| u, user | N | ACL_USER_OBJ | File owner (user) |
| u, user | Y | ACL_USER | Specified user |
| g, group | N | ACL_GROUP_OBJ | File group |
| g, group | Y | ACL_GROUP | Specified group |
| m, mask | N | ACL_MASK | Mask for group class |
| o, other | N | ACL_OTHER | Other users |

## The ACL API
![17-2.png](img/17-2.png)