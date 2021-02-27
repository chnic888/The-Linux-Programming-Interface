#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

int func(int arg) {
    fprintf(stdout, "func arg = %d\n", arg);
    return 10 * arg;
}

int main(int argc, char *argv[]) {
    pid_t childPid;
    int status;

    fprintf(stdout, "PARENT ppid=%d, pid=%d\n", getppid(), getpid());
    childPid = fork();
    if (childPid == -1) {
        fprintf(stderr, "failed to call fork(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if (childPid == 0) {
        fprintf(stdout, "CHILD ppid=%d, pid=%d\n", getppid(), getpid());
        sleep(3);
        exit(func(10));
    }

    if (wait(&status) == -1) {
        fprintf(stderr, "failed to call wait(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    fprintf(stdout, "get child process status %d\n", status);
    return (EXIT_SUCCESS);
}