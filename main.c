#include "commonLibs.h"
#include "memUsage.c"
#include "sessionUsers.c"
#include "cores.c"
#include "machineInfo.c"

int main() {
    printMemUsage();
    printUsers();
    logCores();

    // System information
    printMachineInfo();
}