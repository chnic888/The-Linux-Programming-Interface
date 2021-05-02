## static library
```shell
cd code/

gcc -g -c *.c

ar rcs libhello.a *.o

mv libhello.a ../lib/

cd ..

gcc -g src/main.c -Iinclude/ -Llib/ -lhello -o greeting-static

./greeting-static
```

## shared library
```shell
cd code/

gcc -g -c -fPIC -Wall *.c

gcc -g -shared -Wl,-soname,libhello.so.1 -o libhello.so.1.0.0 *.o

#查看SONAME
objdump -p libhello.so.1.0.0  | grep SONAME

#查看SONAME
readelf -d libhello.so.1.0.0 | grep SONAME

mv libhello.so.1.0.0 ../lib/

cd ..

gcc -g -Wall -Iinclude/ -o hello-shared src/main.c lib/libhello.so.1.0.0

cd lib/

ln -s libhello.so.1.0.0 libhello.so.1

cd ..

export LD_LIBRARY_PATH=./lib/:$LD_LIBRARY_PATH

./hello-shared
```