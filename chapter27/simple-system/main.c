#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int status;
    pid_t childPid;

    switch (childPid = fork()) {
        case -1:
            return -1;
        case 0:
            fprintf(stdout, "child\n");
            execl("/bin/sh", "sh", "-c", argv[1], (char *) NULL);
            fprintf(stdout, "child done\n");
            _exit(127);
        default:
            fprintf(stdout, "main\n");
            if (waitpid(childPid, &status, 0) == -1)
                return -1;
            else
                return status;
    }
}