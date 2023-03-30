#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sysinfo.h>  

double memory_utilization() {
    struct sysinfo info;
    sysinfo(&info);
    double used_memory = info.totalram - info.freeram;
    double total_memory = info.totalram;
    return used_memory / total_memory;
}

int main() {
    int samples = 10;
    int tdelay = 1;
    printf("Nbr of samples: %d -- every %d secs\n", samples, tdelay);
    printf("Memory usage: %lu kilobytes\n", sizeof(struct sysinfo));
    printf("---------------------------------------\n");
    printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");
    int pipefd[2];
    pipe(pipefd);
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        close(pipefd[0]);
        for (int i = 0; i < samples; i++) {
            sleep(tdelay);
            double mem_util = memory_utilization();
            struct sysinfo info;
            sysinfo(&info);
            double used_memory = info.totalram - info.freeram;
            double total_memory = info.totalram;
            double used_virtual_memory = info.totalram - info.freeram + info.totalswap - info.freeswap;
            double total_virtual_memory = info.totalram + info.totalswap;
            write(pipefd[1], &used_memory, sizeof(used_memory));
            write(pipefd[1], &total_memory, sizeof(total_memory));
            write(pipefd[1], &used_virtual_memory, sizeof(used_virtual_memory));
            write(pipefd[1], &total_virtual_memory, sizeof(total_virtual_memory));
        }
        close(pipefd[1]);
    } else {
        // Parent process
        close(pipefd[1]);
        for (int i = 0; i < samples; i++) {
            double used_memory, total_memory, used_virtual_memory, total_virtual_memory;
            read(pipefd[0], &used_memory, sizeof(used_memory));
            read(pipefd[0], &total_memory, sizeof(total_memory));
            read(pipefd[0], &used_virtual_memory, sizeof(used_virtual_memory));
            read(pipefd[0], &total_virtual_memory, sizeof(total_virtual_memory));
            printf("%.2f GB / %.2f GB  -- %.2f GB / %.2f GB\n",
                   used_memory / (1024 * 1024 * 1024),
                   total_memory / (1024 * 1024 * 1024),
                   used_virtual_memory / (1024 * 1024 * 1024),
                   total_virtual_memory / (1024 * 1024 * 1024));
        }
        close(pipefd[0]);
        wait(NULL);
    }
    printf("---------------------------------------\n");
    return 0;
}