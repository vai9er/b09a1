#include "commonLibs.h"

void parseArgs(int argc, char** argv, int* systemm, int* user, int* sequential, int* samples, int* tdelay) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--system") == 0) {
            *systemm = 1;
        } else if (strcmp(argv[i], "--user") == 0) {
            *user = 1;
        } else if (strcmp(argv[i], "--sequential") == 0) {
            * sequential = 1;
        } else if (strncmp(argv[i], "--samples=", 10) == 0) {
            *samples = atoi(argv[i] + 10);
        } else if (strncmp(argv[i], "--tdelay=", 9) == 0) {
            *tdelay = atoi(argv[i] + 9);
        } else if (i == argc - 2) {
            * samples = atoi(argv[i]);
            * tdelay = atoi(argv[i + 1]);
        } else {
            printf("Invalid argument: %s\n", argv[i]);
            exit(1);
        }
    }
}