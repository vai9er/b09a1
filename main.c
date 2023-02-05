#include "commonLibs.h"
#include "memUsage.c"
#include "sessionUsers.c"
#include "cores.c"
#include "machineInfo.c"
#include "parseArg.c"


int main(int argc, char** argv){
    int systemm = 0;
    int user = 0;
    int samples = 10;
    int tdelay = 1;
    int sequential = 0;
    
    parseArgs(argc, argv, &systemm, &user, &sequential, &samples, &tdelay);
    printMemUsage(samples, tdelay);
    printUsers();
    logCores();
    logCpuUsage();
    // // System information
    printMachineInfo();
}