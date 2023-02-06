#include "commonLibs.h"
#include "memUsage.c"
#include "sessionUsers.c"
#include "cores.c"
#include "machineInfo.c"
#include "parseArg.c"

void seqFlag(int samples, int tdelay){
    int j = 0;
    for (int i = 0; i < samples; i++) {
        printf(">>> ITERATION %d\n", i+1);
        printf("Number of samples: %d -- every %d secs\n", samples, tdelay);

        //step 1: get system information
        struct sysinfo systemInfo;
        sysinfo(&systemInfo);
    
        // Get total and used memory
        float memory_total = systemInfo.totalram;
        float memory_used = systemInfo.totalram - systemInfo.freeram;

        // Print memory usage in kilobytes
        printf("Memory usage: %f kilobytes\n", memory_used / 1024);
        printf("---------------------------------------\n");
        printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");


        for (j = 0; j < i; j++){
            printf("%.2f GB / %.2f GB  -- %.2f GB / %.2f GB\n", memory_used / (1024 * 1024 * 1024), memory_total / (1024 * 1024 * 1024), memory_used / (1024 * 1024 * 1024), (memory_total + systemInfo.totalswap) / (1024 * 1024 * 1024));
        }
        printf("%.2f GB / %.2f GB  -- %.2f GB / %.2f GB\n", memory_used / (1024 * 1024 * 1024), memory_total / (1024 * 1024 * 1024), memory_used / (1024 * 1024 * 1024), (memory_total + systemInfo.totalswap) / (1024 * 1024 * 1024));
        for (j = i; j < 9; j++){
            printf("\n");
        }
        printUsers();
        logCores();
        logCpuUsage();
        printMachineInfo();
        sleep(tdelay);
    }
}

void printMe( int NUM_SAMPLES, int SLEEP_TIME) {
    printf("Number of samples: %d -- every %d secs\n", NUM_SAMPLES, SLEEP_TIME);

    //step 1: get system information
    struct sysinfo systemInfo;
    sysinfo(&systemInfo);
    
    // Get total and used memory
    float memory_total = systemInfo.totalram;
    float memory_used = systemInfo.totalram - systemInfo.freeram;

    // Print memory usage in kilobytes

    for (int i = 0; i < NUM_SAMPLES; i++) {
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);

        long memory_usage_kb = usage.ru_maxrss;
        printf("Memory usage: %ld kilobytes\n", memory_usage_kb);
        printf("---------------------------------------\n");
        printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");
        printf("\033[2J"); 
        printf("\033[%d;%dH", 0, 0); 
        for (int j = 0; j < i; j++){
            printf("\n");
        }
        printf("%.2f GB / %.2f GB  -- %.2f GB / %.2f GB\n", memory_used / (1024 * 1024 * 1024), memory_total / (1024 * 1024 * 1024), memory_used / (1024 * 1024 * 1024), (memory_total + systemInfo.totalswap) / (1024 * 1024 * 1024));
        for (int j = i; j < 9; j++){
            printf("\n");
        }
        printUsers();
        logCores();
        logCpuUsage();
        printMachineInfo();
        sleep(SLEEP_TIME);
    }


    printf("---------------------------------------\n");

}

int main(int argc, char** argv){
    int systemm = 0;
    int user = 0;
    int samples = 10;
    int tdelay = 1;
    int sequential = 0;
    
    parseArgs(argc, argv, &systemm, &user, &sequential, &samples, &tdelay);
    if (systemm == 1) {
        printMemUsage(samples, tdelay);
        logCores();
        logCpuUsage();
        printMachineInfo();
    } 
    else if (user == 1) {
        printUsers();
    } 
    else if (sequential == 1) {

        //warnng: give it some time if you are trying to output to a file
        seqFlag(samples,tdelay);

    } 
    else {
        printMe(samples,tdelay);

    }
}