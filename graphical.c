#include "commonLibs.h"
#define PROC_STAT "/proc/stat"

void get_cpu_times(unsigned long long *idle, unsigned long long *total) {
    FILE *fp;
    unsigned long long user, nice, sys, irq, softirq, steal;
    unsigned long long ioWait;

    fp = fopen(PROC_STAT, "r");
    if (fp == NULL) {
        perror("Failed to open " PROC_STAT);
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "cpu %llu %llu %llu %llu %llu %llu %llu %llu", &user, &nice, &sys, idle, &ioWait, &irq, &softirq, &steal);
    fclose(fp);

    *total = user + nice + sys + *idle + ioWait + irq + softirq + steal;
}

double get_cpu_utilization() {
    unsigned long long idle1, total1;
    unsigned long long idle2, total2;

    get_cpu_times(&idle1, &total1);
    sleep(1);
    get_cpu_times(&idle2, &total2);

    double idleDelta = idle2 - idle1;
    double totalDelta = total2 - total1;

    return 1.0 - idleDelta / totalDelta;
}

void refresh2(int samples, int tdelay){
    clear_screen();
    int j = 0;
    float prev_memory_used = 0;
    float memory_used[samples];
    for (int i = 0; i < samples; i++) {
        double cpu_utilization = 100 * get_cpu_utilization();
        printf("Number of samples: %d -- every %d secs\n", samples, tdelay);
        
        //step 1: get system information
        struct sysinfo systemInfo;
        sysinfo(&systemInfo);
    
        // Get total and used memory
        float memory_total = systemInfo.totalram;
        memory_used[i] = systemInfo.totalram - systemInfo.freeram;

        // Print memory usage in kilobytes
        printf("Memory usage: %.0f kilobytes\n", (memory_used[i] / 1024)/1024);
        printf("---------------------------------------\n");
        printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");

        for (j = 0; j <= i; j++){
            printf("%.2f GB / %.2f GB  -- %.2f GB / %.2f GB", memory_used[j] / (1024 * 1024 * 1024), memory_total / (1024 * 1024 * 1024), memory_used[j] / (1024 * 1024 * 1024), (memory_total + systemInfo.totalswap) / (1024 * 1024 * 1024));

            float change = memory_used[j] - prev_memory_used;
            if (change == 0) {
                printf(" |o %.2f (%.2f)\n", change / (1024 * 1024 * 1024), memory_used[j] / (1024 * 1024 * 1024));
            } else if (change > 0) {
                int num_hashes = change / (memory_total / (samples*10));
                printf(" |");
                for (int k = 0; k < num_hashes; k++) {
                    printf("#");
                }
                printf("* %.2f (%.2f)\n", change / (1024 * 1024 * 1024), memory_used[j] / (1024 * 1024 * 1024));
            } else {
                int num_hashes = -change / (memory_total / (samples*10));
                printf(" |");
                for (int k = 0; k < num_hashes; k++) {
                    printf("@");
                }
                printf("* %.2f (%.2f)\n", change / (1024 * 1024 * 1024), memory_used[j] / (1024 * 1024 * 1024));
            }

            prev_memory_used = memory_used[j];
        }

        // Print memory usage in kilobytes after
        for (j = i+1; j < samples; j++){
            printf("\n");
        }
        printf("total cpu use = %.2f%%\n", cpu_utilization);

        //print the rest of the information
        printUsers();
        
        ///FINISH THIS PART
        
        printMachineInfo();
        
        sleep(tdelay);
        clear_screen();
    }
}