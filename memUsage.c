#include "commonLibs.h"

void printMemUsage() {
    printf("Nbr of samples: %d -- every %d secs\n", NUM_SAMPLES, SLEEP_TIME);

    // Memory usage
    struct sysinfo info;
    sysinfo(&info);
    long memory_total = info.totalram;
    long memory_used = info.totalram - info.freeram;
    printf("Memory usage: %lld kilobytes\n", memory_used / 1024);
    printf("---------------------------------------\n");
    printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");

    for (int i = 0; i < NUM_SAMPLES; i++) {
        sysinfo(&info);
        memory_total = info.totalram;
        memory_used = info.totalram - info.freeram;
        printf("%lld GB / %lld GB  -- %lld GB / %lld GB\n", memory_used / (1024 * 1024), memory_total / (1024 * 1024), memory_used / (1024 * 1024), (memory_total + info.totalswap) / (1024 * 1024));
        sleep(SLEEP_TIME);
    }
    printf("---------------------------------------\n");

}