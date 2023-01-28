#include "commonLibs.h"

void printUsers(){
    printf("### Sessions/users ### \n");

    // Sessions/users
    struct utmp *utmp_entry;
    setutent();
    while ( (utmp_entry = getutent()) != NULL) {
        if (utmp_entry->ut_type == USER_PROCESS) {
            printf(" %s\t %s (%s)\n", utmp_entry->ut_user, utmp_entry->ut_line, utmp_entry->ut_host);
        }
    }
    endutent();

    printf("---------------------------------------\n");
    printf("Number of cores: %d\n", sysconf(_SC_NPROCESSORS_ONLN));
}