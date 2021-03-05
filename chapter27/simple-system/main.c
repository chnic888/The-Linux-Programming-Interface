#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    int status;
    pid_t childPid;

    switch (childPid = fork()) {
        case -1:
            return -1;
        case 0:
            execl("/bin/sh", "sh", "-c", argv[1], (char *) NULL);
            _exit(127);
        default:
            if (waitpid(childPid, &status, 0) == -1)
                return -1;
            else
                return status;
    }
}