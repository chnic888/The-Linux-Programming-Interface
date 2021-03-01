```shell
./build/chapter26/child-status/26-child-status 23
```

```shell
./build/chapter26/child-status/26-child-status &
kill -STOP cpid
kill -CONT cpid
kill -ABRT cpid
```

```shell
ulimit -c unlimited
./build/chapter26/child-status/26-child-status &
kill -ABRT cpid
```