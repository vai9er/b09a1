#include "commonLibs.h"


void printMachineInfo(){

    printf("### System Information ### \n");
    struct utsname compInfo;
    uname(&compInfo);
    printf("SYSTEM NAME: %s \n", compInfo.sysname);
    printf("NODE NAME: %s \n", compInfo.nodename);
    printf("RELEASE: %s \n", compInfo.release);
    printf("VERSION: %s \n", compInfo.version);
    printf("MACHINE: %s \n", compInfo.machine);
    printf("---------------------------------------\n");

}