#include "commonLibs.h"

//this function prints the users and sessions using the utmp file
//the utmp file is a file that stores information about the users and sessions
void logSessional(struct utmp *userSession){
    //print the sessions
    while ( (userSession = getutent()) != NULL) {
        if (userSession->ut_type == USER_PROCESS) printf(" %s   %s (%s)\n", userSession->ut_user, userSession->ut_line, userSession->ut_host);
    }
}


//this function prints the users and sessions using the utmp file and the getutent() function
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

