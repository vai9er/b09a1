#include "commonLibs.h"

void printMemUsage( int NUM_SAMPLES, int SLEEP_TIME) {
    printf("Number of samples: %d -- every %d secs\n", NUM_SAMPLES, SLEEP_TIME);

    //step 1: get system information
    struct sysinfo systemInfo;
    sysinfo(&systemInfo);
    
    // Get total and used memory
    float memory_total = systemInfo.totalram;
    float memory_used = systemInfo.totalram - systemInfo.freeram;

    // Print memory usage in kilobytes
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    // Print memory usage in kilobytes
    long memory_usage_kb = usage.ru_maxrss;
    printf("Memory usage: %ld kilobytes\n", memory_usage_kb);
    printf("---------------------------------------\n");
    printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");

    for (int i = 0; i < NUM_SAMPLES; i++) {

        //use .2f as specifier- no more than 2 decimal places to mimic the handout
        printf("%.2f GB / %.2f GB  -- %.2f GB / %.2f GB\n", memory_used / (1024 * 1024 * 1024), memory_total / (1024 * 1024 * 1024), memory_used / (1024 * 1024 * 1024), (memory_total + systemInfo.totalswap) / (1024 * 1024 * 1024));
        
        sleep(SLEEP_TIME);
    }


    printf("---------------------------------------\n");

}

