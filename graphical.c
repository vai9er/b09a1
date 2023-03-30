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

double get_cpu_utilization(int tdelay) {
    unsigned long long idle1, total1;
    unsigned long long idle2, total2;

    get_cpu_times(&idle1, &total1);
    sleep(tdelay);
    get_cpu_times(&idle2, &total2);

    double idleDelta = idle2 - idle1;
    double totalDelta = total2 - total1;

    return 1.0 - idleDelta / totalDelta;
}

double get_cpu_utilization2() {
    unsigned long long idle1, total1;
    unsigned long long idle2, total2;

    get_cpu_times(&idle1, &total1);
    sleep(1);
    get_cpu_times(&idle2, &total2);

    double idleDelta = idle2 - idle1;
    double totalDelta = total2 - total1;

    return 1.0 - idleDelta / totalDelta;
}

// void refresh2(int samples, int tdelay){
//     clear_screen();
//     int j = 0;
//     float prev_memory_used = 0;
//     float memory_used[samples];
//     for (int i = 0; i < samples; i++) {
//         double cpu_utilization = 100 * get_cpu_utilization();
//         printf("Number of samples: %d -- every %d secs\n", samples, tdelay);
        
//         //step 1: get system information
//         struct sysinfo systemInfo;
//         sysinfo(&systemInfo);
    
//         // Get total and used memory
//         float memory_total = systemInfo.totalram;
//         memory_used[i] = systemInfo.totalram - systemInfo.freeram;

//         // Print memory usage in kilobytes
//         printf("Memory usage: %.0f kilobytes\n", (memory_used[i] / 1024)/1024);
//         printf("---------------------------------------\n");
//         printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");

//         for (j = 0; j <= i; j++){
//             printf("%.2f GB / %.2f GB  -- %.2f GB / %.2f GB", memory_used[j] / (1024 * 1024 * 1024), memory_total / (1024 * 1024 * 1024), memory_used[j] / (1024 * 1024 * 1024), (memory_total + systemInfo.totalswap) / (1024 * 1024 * 1024));

//             float change = memory_used[j] - prev_memory_used;
//             if (change == 0) {
//                 printf(" |o %.2f (%.2f)\n", change / (1024 * 1024 * 1024), memory_used[j] / (1024 * 1024 * 1024));
//             } else if (change > 0) {
//                 int num_hashes = change / (memory_total / (samples*10));
//                 printf(" |");
//                 for (int k = 0; k < num_hashes; k++) {
//                     printf("#");
//                 }
//                 printf("* %.2f (%.2f)\n", change / (1024 * 1024 * 1024), memory_used[j] / (1024 * 1024 * 1024));
//             } else {
//                 int num_hashes = -change / (memory_total / (samples*10));
//                 printf(" |");
//                 for (int k = 0; k < num_hashes; k++) {
//                     printf("@");
//                 }
//                 printf("* %.2f (%.2f)\n", change / (1024 * 1024 * 1024), memory_used[j] / (1024 * 1024 * 1024));
//             }

//             prev_memory_used = memory_used[j];
//         }

//         // Print memory usage in kilobytes after
//         for (j = i+1; j < samples; j++){
//             printf("\n");
//         }
//         printf("total cpu use = %.2f%%\n", cpu_utilization);

//         //print the rest of the information
//         printUsers();
        
        
//         printMachineInfo();
        
//         sleep(tdelay);
//         clear_screen();
//     }
// }

void printMemoryUtilization(int samples, float memory_used[], float memory_total, float prev_memory_used) {
    printf("---------------------------------------\n");
    printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");

    for (int j = 0; j < samples; j++){
        struct sysinfo systemInfo;
        sysinfo(&systemInfo);
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
}

void printCPUUtilization(int samples, int tdelay) {
    double prev_cpu_utilization = 100 * get_cpu_utilization2();
    int prev_num_bars = 3;

    for (int i = 0; i < samples; i++) {
        double cpu_utilization = 100 * get_cpu_utilization2();
        int num_bars = prev_num_bars + (int)((cpu_utilization - prev_cpu_utilization) / 0.50);
        if (num_bars > 0) {
            printf("\t");
            for (int j = 0; j < num_bars; j++) {
                printf("|");
            }
            printf(" %.2f\n", cpu_utilization);
        } else {
            printf("\t %.2f\n", cpu_utilization);
        }
        prev_cpu_utilization = cpu_utilization;
        prev_num_bars = num_bars;
        sleep(tdelay);
    }
}

// void refresh2(int samples, int tdelay){
//     clear_screen();
//     int j = 0;
//     float prev_memory_used = 0;
//     float memory_used[samples];
//     int prev_num_bars = 3;
//     double prev_cpu_utilization = 100 * get_cpu_utilization2();
//     for (int i = 0; i < samples; i++) {
//         double cpu_utilization = 100 * get_cpu_utilization(tdelay);
//         printf("Nbr of samples: %d -- every %d secs\n", samples, tdelay);
        
//         //step 1: get system information
//         struct sysinfo systemInfo;
//         sysinfo(&systemInfo);
    
//         // Get total and used memory
//         float memory_total = systemInfo.totalram;
//         memory_used[i] = systemInfo.totalram - systemInfo.freeram;

//         // Print memory usage in kilobytes
//         printf("Memory usage: %.0f kilobytes\n", (memory_used[i] / 1024)/1024);

//         printMemoryUtilization(i+1, memory_used, memory_total, prev_memory_used);

//         // Print memory usage in kilobytes after
//         for (j = i+1; j < samples; j++){
//             printf("\n");
//         }
        
//         printf("---------------------------------------\n");
//         printf("Number of cores: %d \n", get_nprocs());
//         printf("total cpu use = %.2f%%\n", cpu_utilization);
        
//         // printCPUUtilization(samples, tdelay);
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

//         //print the rest of the information
//         printUsers();
        
        
//         printMachineInfo();
    
//         clear_screen();
//     }
// }
typedef struct {
    double utilization;
    int num_bars;
} cpu_sample_t;

void refresh2(int samples, int tdelay){
    clear_screen();
    int j = 0;
    float prev_memory_used = 0;
    float memory_used[samples];
    cpu_sample_t cpu_utilization[samples];
    for (int i = 0; i < samples; i++) {
        cpu_utilization[i].utilization = 100 * get_cpu_utilization(tdelay);
        cpu_utilization[i].num_bars = (int)(cpu_utilization[i].utilization / 0.20);
        printf("Nbr of samples: %d -- every %d secs\n", samples, tdelay);
        
        //step 1: get system information
        struct sysinfo systemInfo;
        sysinfo(&systemInfo);
    
        // Get total and used memory
        float memory_total = systemInfo.totalram;
        memory_used[i] = systemInfo.totalram - systemInfo.freeram;

        // Print memory usage in kilobytes
        printf("Memory usage: %.0f kilobytes\n", (memory_used[i] / 1024)/1024);

        printMemoryUtilization(i+1, memory_used, memory_total, prev_memory_used);

        // Print memory usage in kilobytes after
        for (j = i+1; j < samples; j++){
            printf("\n");
        }
        
        printf("---------------------------------------\n");
        printf("Number of cores: %d \n", get_nprocs());
        printf("total cpu use = %.2f%%\n", cpu_utilization[i].utilization);
        
        // printCPUUtilization(samples, tdelay);
        for (int k = 0; k <= i; k++) {
            if (cpu_utilization[k].num_bars > 0) {
                printf("\t");
                for (int j = 0; j < cpu_utilization[k].num_bars; j++) {
                    printf("|");
                }
                printf(" %.2f\n", cpu_utilization[k].utilization);
            } else {
                printf("\t %.2f\n", cpu_utilization[k].utilization);
            }
        }

        //print the rest of the information
        printUsers();
        
        
        printMachineInfo();

        clear_screen();
    }
}

// void refresh2(int samples, int tdelay){
//     int memory_pipe[2];
//     int cpu_pipe[2];
//     int users_pipe[2];

//     if (pipe(memory_pipe) == -1 || pipe(cpu_pipe) == -1 || pipe(users_pipe) == -1) {
//         perror("pipe");
//         exit(EXIT_FAILURE);
//     }

//     pid_t memory_pid = fork();
//     if (memory_pid == -1) {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     }

//     if (memory_pid == 0) {
//         // Memory child process
//         close(memory_pipe[0]);
//         dup2(memory_pipe[1], STDOUT_FILENO);
//         close(memory_pipe[1]);

//         float prev_memory_used = 0;
//         float memory_used[samples];
//         for (int i = 0; i < samples; i++) {
//             struct sysinfo systemInfo;
//             sysinfo(&systemInfo);

//             float memory_total = systemInfo.totalram;
//             memory_used[i] = systemInfo.totalram - systemInfo.freeram;

//             printf("Memory usage: %.0f kilobytes\n", (memory_used[i] / 1024)/1024);

//             printMemoryUtilization(i+1, memory_used, memory_total, prev_memory_used);

//             for (int j = i+1; j < samples; j++){
//                 printf("\n");
//             }

//             prev_memory_used = memory_used[i];
//             sleep(tdelay);
//         }
//         exit(EXIT_SUCCESS);
//     }

//     pid_t cpu_pid = fork();
//     if (cpu_pid == -1) {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     }

//     if (cpu_pid == 0) {
//         // CPU child process
//         close(cpu_pipe[0]);
//         dup2(cpu_pipe[1], STDOUT_FILENO);
//         close(cpu_pipe[1]);

//         for (int i = 0; i < samples; i++) {
//             double cpu_utilization = 100 * get_cpu_utilization();
//             printf("---------------------------------------\n");
//             printf("Number of cores: %d \n", get_nprocs());
//             printf("total cpu use = %.2f%%\n", cpu_utilization);

//             printCPUUtilization(samples, tdelay);

//             sleep(tdelay);
//         }
//         exit(EXIT_SUCCESS);
//     }

//     pid_t users_pid = fork();
//     if (users_pid == -1) {
//         perror("fork");
//         exit(EXIT_FAILURE);
//     }

//     if (users_pid == 0) {
//         // Users child process
//         close(users_pipe[0]);
//         dup2(users_pipe[1], STDOUT_FILENO);
//         close(users_pipe[1]);

//         for (int i = 0; i < samples; i++) {
//             printUsers();
//             sleep(tdelay);
//         }
//         exit(EXIT_SUCCESS);
//     }

//     // Parent process
//     close(memory_pipe[1]);
//     close(cpu_pipe[1]);
//     close(users_pipe[1]);

//     FILE *memory_stream = fdopen(memory_pipe[0], "r");
//     FILE *cpu_stream = fdopen(cpu_pipe[0], "r");
//     FILE *users_stream = fdopen(users_pipe[0], "r");

//     char buffer[BUFSIZ];
//     for (int i = 0; i < samples; i++) {
//         clear_screen();
//         printf("Nbr of samples: %d -- every %d secs\n", samples, tdelay);

//         while (fgets(buffer, BUFSIZ, memory_stream) != NULL && strcmp(buffer, "\n") != 0) {
//             printf("%s", buffer);
//         }

//         while (fgets(buffer, BUFSIZ, cpu_stream) != NULL && strcmp(buffer, "\n") != 0) {
//             printf("%s", buffer);
//         }
//         while (fgets(buffer, BUFSIZ, users_stream) != NULL && strcmp(buffer, "\n") != 0) {
//             printf("%s", buffer);
//         }

//         printMachineInfo();

//         sleep(tdelay);
//     }

//     fclose(memory_stream);
//     fclose(cpu_stream);
//     fclose(users_stream);

//     waitpid(memory_pid, NULL, 0);
//     waitpid(cpu_pid, NULL, 0);
//     waitpid(users_pid, NULL, 0);
// }