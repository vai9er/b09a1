#include "commonLibs.h"


void logCores(){

    printf("-------------------------------------\n");
    printf("Number of cores: %d\n", sysconf(_SC_NPROCESSORS_ONLN));
    
}