#include "readv.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int fd = open("aa.txt", O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    int a = 101233;
    write(fd, &a, sizeof(a));
    close(fd);

    int fd2 = open("aa.txt", O_RDONLY);
    int b;
    read(fd, &b, sizeof(b));
    close(fd2);

    printf("%d", b);

    return 0;
}