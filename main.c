#include "commonLibs.h"
#include "memUsage.c"
#include "sessionUsers.c"

int main() {
    printMemUsage();
    printUsers();
    // CPU usage
    printf("### CPU ###\n");
    printf("Number of cores: %d\n", sysconf(_SC_NPROCESSORS_ONLN));
    printf("---------------------------------------\n");
    // System information
    printf("### System Information ###");
    struct utsname uname_pointer;
    uname(&uname_pointer);
    printf("System name: %s", uname_pointer.sysname);
    printf("Node name: %s", uname_pointer.nodename);
    printf("Release: %s", uname_pointer.release);
    printf("Version: %s", uname_pointer.version);
    printf("Machine: %s", uname_pointer.machine);
    printf("---------------------------------------\n");
    return 0;
    
}