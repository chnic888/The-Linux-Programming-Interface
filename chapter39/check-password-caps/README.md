```shell
sudo setcap "cap_dac_read_search=p" ./build/chapter39/check-password-caps/39-check-password-caps

getcap ./build/chapter39/check-password-caps/39-check-password-caps

./build/chapter39/check-password-caps/39-check-password-caps
```