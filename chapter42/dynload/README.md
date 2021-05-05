```shell
gcc -g -c -fPIC -Wall *.c

gcc -g -shared -Wl,-soname,libgreeting.so.1 -o libgreeting.so.1.0.0 *.o

mv libgreeting.so.1.0.0 ../lib/

cd ..

gcc -o dynload src/main.c -ldl

./dynload ./lib/libgreeting.so.1.0.0 greeting

//or 
export LD_LIBRARY_PATH=./lib/:$LD_LIBRARY_PATH

./dynload libgreeting.so.1.0.0 greeting
```