```shell
cd code/

gcc -c *.c

ar rcs libhello.a *.o

mv libhello.a ../lib/

cd ..

gcc src/main.c -Iinclude/  -Llib/ -lhello -o greeting.out

./greeting.out
```