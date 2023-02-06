#include "commonLibs.h"
#include "memUsage.c"
#include "sessionUsers.c"
#include "cores.c"
#include "machineInfo.c"
#include "parseArg.c"

//clears the screen with ANSI escape codes
void clear_screen() {
  printf("\033[2J");  // clear entire screen
  printf("\033[%d;%dH", 0, 0);  // move cursor to the top-left corner
}

//function to print the menu
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
        printMachineInfo();
        logCpuUsage();
        sleep(tdelay);
        printf("\n");
    }
}

void refresh(int samples, int tdelay){
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
        printf("Memory usage: %.0f kilobytes\n", (memory_used / 1024)/1024);
        printf("---------------------------------------\n");
        printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");

        // Print memory usage in kilobytes before
        for (j = 0; j < i; j++){
            printf("%.2f GB / %.2f GB  -- %.2f GB / %.2f GB\n", memory_used / (1024 * 1024 * 1024), memory_total / (1024 * 1024 * 1024), memory_used / (1024 * 1024 * 1024), (memory_total + systemInfo.totalswap) / (1024 * 1024 * 1024));
        }
        // Print current memory usage in kilobytes
        printf("%.2f GB / %.2f GB  -- %.2f GB / %.2f GB\n", memory_used / (1024 * 1024 * 1024), memory_total / (1024 * 1024 * 1024), memory_used / (1024 * 1024 * 1024), (memory_total + systemInfo.totalswap) / (1024 * 1024 * 1024));

        // Print memory usage in kilobytes after
        for (j = i; j < 9; j++){
            printf("\n");
        }
        //print the rest of the information
        printUsers();
        logCores();
        printMachineInfo();
        logCpuUsage();
        clear_screen();
        
        //gn
        sleep(tdelay);
    }
}

int main(int argc, char** argv){
    //default values
    int systemm = 0;
    int user = 0;
    int samples = 10;
    int tdelay = 1;
    int sequential = 0;
    
    //parse the arguments
    parseArgs(argc, argv, &systemm, &user, &sequential, &samples, &tdelay);

    //--system
    if (systemm == 1) {
        printMemUsage(samples, tdelay);
        logCores();
        logCpuUsage();
        printMachineInfo();
    } 

    //--user
    else if (user == 1) {
        printUsers();
    } 

    //--sequential
    else if (sequential == 1) {

        //warnng: give it some time if you are trying to output to a file
        seqFlag(samples,tdelay);

    } 
    else {
        //refresh-like output
        refresh(samples,tdelay);

    }
}