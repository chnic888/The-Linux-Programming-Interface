```shell script
./build/chapter13/direct-read/13-direct-read ./chapter13/direct-read/data.txt 512 | cat -n

./build/chapter13/direct-read/13-direct-read ./chapter13/direct-read/data.txt 1024 | cat -n

./build/chapter13/direct-read/13-direct-read ./chapter13/direct-read/data.txt 256 | cat -n

./build/chapter13/direct-read/13-direct-read ./chapter13/direct-read/data.txt 1024 1 | cat -n

./build/chapter13/direct-read/13-direct-read ./chapter13/direct-read/data.txt 512 512 | cat -n

./build/chapter13/direct-read/13-direct-read ./chapter13/direct-read/data.txt 512 0 1024 | cat -n
```