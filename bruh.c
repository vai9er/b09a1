#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>

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

void cpuinfo(){
   double cpu_utilization = 100 * get_cpu_utilization();
   printf("total cpu use = %.2f%%\n", cpu_utilization);
}

// int main() {
//     int num_cores = get_nprocs();
//     printf("Number of cores: %d\n", num_cores);

//     double cpu_utilization = 100 * get_cpu_utilization();
//     printf("total cpu use = %.2f%%\n", cpu_utilization);

//     double prev_cpu_utilization = cpu_utilization;
//     int prev_num_bars = 3;

//     for (int i = 0; i < num_cores; i++) {
//         int num_bars = prev_num_bars + (int)((cpu_utilization - prev_cpu_utilization) / 0.20);
//         if (num_bars > 0) {
//             printf("\t");
//             for (int j = 0; j < num_bars; j++) {
//                 printf("|");
//             }
//             printf(" %.2f\n", cpu_utilization);
//         } else {
//             printf("\t %.2f\n", cpu_utilization);
//         }
//         prev_cpu_utilization = cpu_utilization;
//         prev_num_bars = num_bars;
//         cpu_utilization = 100 * get_cpu_utilization();
//     }

//     return 0;
// }