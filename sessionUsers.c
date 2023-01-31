#include "commonLibs.h"

void logSessional(struct utmp *userSession){
    while ( (userSession = getutent()) != NULL) {
        if (userSession->ut_type == USER_PROCESS) printf(" %s   %s (%s)\n", userSession->ut_user, userSession->ut_line, userSession->ut_host);
    }
}


void printUsers(){
    printf("### Sessions/users ### \n");

    // get sessional information
    struct utmp *userSession;

    //open file
    setutent();

    logSessional(userSession);

    //close file
    endutent();
}

