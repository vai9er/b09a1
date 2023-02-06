#include "commonLibs.h"

//write a helper function that checks whether a CHAR* is an int
//if it is, return 1, else return 0
int isInt(char *str){
    int i = 0;
    while (str[i] != '\0'){
        if (isdigit(str[i]) == 0){
            return 0;
        }
        i++;
    }
    return 1;
}

void parseArgs(int argc, char** argv, int* systemm, int* user, int* sequential, int* samples, int* tdelay) {
    int positionalArgIndex = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--system") == 0) {
            *systemm = 1;
        } else if (strcmp(argv[i], "--user") == 0) {
            *user = 1;
        } else if (strcmp(argv[i], "--sequential") == 0) {
            *sequential = 1;
        } else if (strncmp(argv[i], "--samples=", 10) == 0) {
            *samples = atoi(argv[i] + 10);
            if (*tdelay <1){
                printf("Invalid arguement: %s\n", argv[i]);
                exit(1);
            }
            if (isInt(argv[i] + 10) == 0){
                printf("Invalid argument: %s -- not an int\n", argv[i]);
                exit(1);
            }
        } else if (strncmp(argv[i], "--tdelay=", 9) == 0) {
            *tdelay = atoi(argv[i] + 9);
            if (*tdelay <1){
                printf("Invalid arguement: %s\n", argv[i]);
                exit(1);
            }
            if (isInt(argv[i] + 9) == 0){
                printf("Invalid argument: %s -- not an int\n", argv[i]);
                exit(1);
            }
        } else if (positionalArgIndex == 0) {
            int value = atoi(argv[i]);
            if (value <= 0) {
                printf("Invalid positional argument: %s\n", argv[i]);
                exit(1);
            }
            if (isInt(argv[i]) == 0){
                printf("Invalid argument: %s -- not an int\n", argv[i]);
                exit(1);
            }
            *samples = value;
            positionalArgIndex++;
        } else if (positionalArgIndex == 1) {
            int value = atoi(argv[i]);
            if (value <= 0) {
                printf("Invalid positional argument: %s\n", argv[i]);
                exit(1);
            }
            if (isInt(argv[i]) == 0){
                printf("Invalid argument: %s -- not an int\n", argv[i]);
                exit(1);
            }
            *tdelay = value;
            positionalArgIndex++;
        } else {
            printf("Invalid argument: %s\n", argv[i]);
            exit(1);
    }
}
if (positionalArgIndex != 0 && positionalArgIndex != 2) {
printf("Two positional arguments are required, but %d were provided\n", positionalArgIndex);
exit(1);
}
}

