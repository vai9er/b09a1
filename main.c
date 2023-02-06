#include "commonLibs.h"
#include "memUsage.c"
#include "sessionUsers.c"
#include "cores.c"
#include "machineInfo.c"
#include "parseArg.c"


void log_cpu_usage() {
    static clock_t last_cpu_time = 0;
    static clock_t last_elapsed_time = 0;

    struct tms current_cpu_times;
    if (times(&current_cpu_times) == (clock_t)-1) {
        perror("times");
        return;
    }

    clock_t current_cpu_time = current_cpu_times.tms_utime + current_cpu_times.tms_stime;
    clock_t current_elapsed_time = clock();

    if (last_cpu_time == 0) {
        last_cpu_time = current_cpu_time;
        last_elapsed_time = current_elapsed_time;
        return;
    }

    clock_t elapsed_time = current_elapsed_time - last_elapsed_time;
    clock_t cpu_time = current_cpu_time - last_cpu_time;

    int usage = (int)(100.0 * ((double)cpu_time / (double)elapsed_time));
    printf("CPU usage: %d%%\n", usage);

    last_cpu_time = current_cpu_time;
    last_elapsed_time = current_elapsed_time;
}

void clear_screen() {
  printf("\033[2J");  // clear entire screen
  printf("\033[%d;%dH", 0, 0);  // move cursor to the top-left corner
}

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
        printf("\n");
    }
}

void printMe(int samples, int tdelay){
    clear_screen();
    int j = 0;
    for (int i = 0; i < samples; i++) {
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
        printMachineInfo();
        logCpuUsage();
        clear_screen();
        

        sleep(tdelay);
    }
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