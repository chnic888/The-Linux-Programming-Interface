#include <unistd.h>
#include <sys/reboot.h>
#include "reboot.h"

int rebootSys() {
    fprintf(stdout, "sync data\n");
    sync();
    fprintf(stdout, "reboot sys\n");
    return reboot(RB_AUTOBOOT);
}