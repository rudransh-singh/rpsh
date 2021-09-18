//contains function which displays the shell prompt <Name@UBUNTU:~>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "colors.h"
#include <string.h>
void display(char *homedirectory) //passing home directory as an argument, since we need to account for ~ sign
{
    char *user_name; //will store the name of the user (in my case "rpsingh")
    user_name = getenv ("USER"); // gets the environment variable "$USER" (can also use LOGNAME)
    
    char system_name[200]=""; //stores the system name
    gethostname(system_name,200); // stores the system name in the variable
    

    char curr_location[200]; // stores the current location of the file 
    getcwd(curr_location,200); // use the getcwd command to store the current path in curr_location
    
    
    //now if the executable is stored in "/path" then "/path/abc/def" must be replaced by "~/abc/def"
    int i=0;
    for(i=0;i<strlen(homedirectory)&&curr_location[i];i++)
    {
        if(curr_location[i]!=homedirectory[i])
            break;
    }
    char finalpath[200];
    if(i==strlen(homedirectory))
    {
        finalpath[0]='~';
        finalpath[1]='\0';
        strcat(finalpath,curr_location+i);
    }
    else
    {
        strcpy(finalpath,curr_location);
    }

    
    
    
   //finally print the prompt 
    printf(BRIGHT CYAN"<%s@%s" YELLOW ":%s> "RESET,user_name,system_name,finalpath);
}