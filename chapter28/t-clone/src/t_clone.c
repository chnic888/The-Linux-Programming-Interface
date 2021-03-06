#define _GNU_SOURCE

#include "t_clone.h"
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#ifndef CHILD_SIG
#define CHILD_SIG SIGUSR1
#endif

static int childFunc(void *arg) {
    if (close(*((int *) arg)) == -1) {
        fprintf(stderr, "failed to call childFunc(), %d\n", errno);
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

int tClone(int argc, char *argv[]) {
    const int STACK_SIZE = 65535;
    char *stack;
    char *stackTop;
    int s, fd, flags;

    fd = open("/dev/null", O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "failed to call open(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    flags = (argc > 1) ? CLONE_FILES : 0;

    stack = malloc(STACK_SIZE);
    if (stack == NULL) {
        fprintf(stderr, "failed to call malloc(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    //assume stack grows downward and get the stack top-end
    stackTop = stack + STACK_SIZE;

    if (CHILD_SIG != 0 && CHILD_SIG != SIGCHLD) {
        if (signal(CHILD_SIG, SIG_IGN) == SIG_ERR) {
            fprintf(stderr, "failed to call signal(), %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }

    if (clone(childFunc, stackTop, flags | CHILD_SIG, (void *) &fd) == -1) {
        fprintf(stderr, "failed to call clone(), %d\n", errno);
        exit(EXIT_FAILURE);
    }

    if (waitpid(-1, NULL, (CHILD_SIG != SIGCHLD) ? __WCLONE : 0) == -1) {
        fprintf(stderr, "failed to call waitpid(), %d\n", errno);
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "child has terminated\n");

    s = write(fd, "x", 1);
    if (s == -1 && errno == EBADF) {
        fprintf(stdout, "file descriptor %d has been closed\n", fd);
    } else if (s == -1) {
        fprintf(stdout, "write() on file descriptor %d failed unexpectedly (%d)\n", fd, errno);
    } else {
        fprintf(stdout, "write() on file descriptor %d succeeded\n", fd);
    }

    exit(EXIT_SUCCESS);
}