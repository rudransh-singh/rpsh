#include "cd.h"
#include "../utils.h"
#include "../shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
char lastpath[MAX_ARG_SIZE]="~";// will store the last visited path(update everytime cd function is invoked)
char lasttolastpath[MAX_ARG_SIZE]="~";
char cdcounter=0; // counts the number of times the cd program has been run
//home stores home path
//command params is the command parameter
//numarg is the number of args, for cd command numarg is just 0 or 1

void cd(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int numarg)
{
    char gotolocation[MAX_ARG_SIZE];
    int valid=0;
    if(numarg>2)
    {
        printf("cd: Too many arguments\n");
        return;
    }
    if(numarg==1)
    {
        strcpy(gotolocation,home);
        if(cdcounter==0)
        {
        strcpy(lastpath,home);
        }
        else
        {
            strcpy(lasttolastpath,lastpath);
            strcpy(lastpath,home);
        }
        valid=chdir(gotolocation);
    }
    else
    {
        strcpy(gotolocation,commandparams[1]);
        if (strcmp(gotolocation,"~")==0)
        {
            strcpy(gotolocation,home);
            if(cdcounter==0)
            { strcpy(lastpath,home); }
             else
        {
            strcpy(lasttolastpath,lastpath);
            strcpy(lastpath,home);
        }
        valid=chdir(gotolocation);
        }
        else if(commandparams[1][0]=='.' && (commandparams[1][1]=='/' || commandparams[1][1]=='\0')) // for cd ./blablabla or cd .
        {
           char currentlocation[MAX_ARG_SIZE];
           getcwd(currentlocation,MAX_ARG_SIZE); 
           char new[MAX_ARG_SIZE];
           strcpy(new,currentlocation);
           strcat(new,gotolocation+1);
           strcpy(gotolocation,new);
           if(cdcounter==0)
           { strcpy(lastpath,new); }
            else
        {
            strcpy(lasttolastpath,lastpath);
            strcpy(lastpath,new);
        }
        valid=chdir(gotolocation);
        }
        else if(commandparams[1][0]=='.' && commandparams[1][1]=='.' && (commandparams[1][2]=='/' || commandparams[1][2]=='\0'))
        {
           char currentlocation[MAX_ARG_SIZE];
           getcwd(currentlocation,MAX_ARG_SIZE); 
           char* ptr=strrchr(currentlocation,'/');//searches for the last "/" in the path name
           *ptr='\0';
           char new[MAX_ARG_SIZE];
           strcpy(new,currentlocation);
           strcat(new,gotolocation+2);
           strcpy(gotolocation,new);
           if(cdcounter==0)
           { strcpy(lastpath,new); }
            else
        {
            strcpy(lasttolastpath,lastpath);
            strcpy(lastpath,new);
        }
        valid=chdir(gotolocation);
        }
        else if(strcmp(gotolocation,"-")==0)
        {
            strcpy(gotolocation,lasttolastpath);
            strcpy(lasttolastpath,lastpath);
            strcpy(lastpath,gotolocation);
            valid=chdir(gotolocation);
        }
        else
        {
            valid=chdir(gotolocation);
            if(valid!=-1)
            {
            char currendirectory[MAX_ARG_SIZE];
            getcwd(currendirectory,MAX_ARG_SIZE);
            if(cdcounter==0)
            { strcpy(lastpath,currendirectory); }
             else
            {
                strcpy(lasttolastpath,lastpath);
                strcpy(lastpath,currendirectory);
            }
            }
        }
    }
    if(valid < 0)
    {
        printf("cd:%s: No such file or directory\n", gotolocation);
    }
    cdcounter++;
    return;
}
