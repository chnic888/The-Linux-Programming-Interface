## TImeZone

```shell script
TZ=":Asia/Shanghai" ./build/chapter10/show-time/10-show-time

TZ=":Pacific/Auckland" ./build/chapter10/show-time/10-show-time
```

## Locale

```shell script
LANG=de_DE ./build/chapter10/show-time/10-show-time

LANG=de_DE LC_TIME=it_IT ./build/chapter10/show-time/10-show-time

LC_ALL=fr_FR LC_TIME=en_US ./build/chapter10/show-time/10-show-time
```