# FILE I/O: THE UNIVERSAL I/O MODEL

## Changing the File Offset: lseek()
![4-1.png](./img/4-1.png)

- SEEK_SET 将文件偏移量设置为从文件头部开始的offset字节，offset不能为负数
- SEEK_CUR 相对于当前的文件偏移量，并将偏移量调整offset和字节，offset可以为负数 
- SEEK_END 将文件偏移量设置为从文件尾部的offset字节，offset可以为负数

- 从文件结尾后到新写入的数据间的这段空间被称为文件空洞(file holes)