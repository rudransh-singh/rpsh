//Standard header files 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>
#include <signal.h>
#include <sys/wait.h>

//user-defined header files
#include "colors.h"
#include "display.h"
#include "commands/pwd.h"
#include "commands/cd.h"
#include "commands/echo.h"
#include "commands/fgbgprocess.h"
#include "commands/ls.h"
#include "commands/pinfo.h"
#include "commands/history.h"
#include "utils.h"


char home[200];
processlist PList;
void handle_child_death(int sig)
{
    int stat;
    pid_t processpid;
    processpid=waitpid(-1, &stat, WNOHANG | WUNTRACED );
    if(processpid > 0)
    {
        char processname[256];
        searchforprocessname(PList,processpid,processname);
        // strcpy(processname,"muguop");
        if(WIFEXITED(stat))
        {
            printf("%s with pid= %d exited normally\n",processname,processpid);
        }
        else
        {
            printf("%s with pid= %d exited abnormally\n",processname,processpid);

        }
        deleteprocess(PList,processpid);
        display(home);
        fflush(stdout);
    }
}

int  openhistoryfile(char history[20][MAX_ARG_SIZE])
{
    FILE* fh;
    fh=fopen("history.txt","r");
    char* temp;//temporary pointer for strtok_r;
    char* token;
    char *buffer;
    size_t buffersize=10240;
    buffer=(char *)malloc(buffersize*sizeof(char));
    getline(&buffer,&buffersize,fh);
    //now buffer stores the contents of the history file
    token=strtok_r(buffer,";",&temp);
    int count=0;
    while(token!=NULL)
    {
        strcpy(history[count],token);
        token=strtok_r(NULL,";",&temp);
        count++;
    }
    fclose(fh);
    return count;
}
//history[0]==oldest instruction, history[19]==newest instruction
void storeinhistory(char inputline[], char history[20][MAX_ARG_SIZE], int *numcommandsstored)
{
    inputline[strlen(inputline)-1]='\0';
    int index= *numcommandsstored;
    if(strcmp(history[index-1],inputline)==0)
        return; 
    if(*(numcommandsstored)==20)
    {
        for(int i=0;i<19;i++)
        {
            strcpy(history[i],history[i+1]);
        }
        strcpy(history[19],inputline);
    }
    else
    {
        strcpy(history[index],inputline);
        (*numcommandsstored)++;
    }
}

void loghistory(char home[200],char history[20][MAX_ARG_SIZE],int numcommandsstored)
{
    char filepath[512];
    strcpy(filepath,home);
    strcat(filepath,"/history.txt");
    FILE* fh;
    fh=fopen(filepath,"w");
    for(int i=0;i<numcommandsstored;i++)
    {
        fprintf(fh,"%s;",history[i]);
    }
    fclose(fh);
}

char historyinfo[20][MAX_ARG_SIZE];// makes a 2d array to store previous commands
int numcommandsstored;

int main()
{   
    

    numcommandsstored= openhistoryfile(historyinfo); // function to open history.txt and store it's contents in history array
    //num commands stored contains the number of commands currently stored in history
    PList=createemptylist();
    getcwd(home,200);
    // strcpy(home,"/home");
    signal(SIGCHLD,handle_child_death);
    while(1)
    {
        char input_line[1024]; // stores the input line (which is taken through readline function)
        char** commandparams; //will store the command info (command name and params) after parsing



        display(home); //displays the prompt
        read_line(input_line); //will read the command line
        storeinhistory(input_line,historyinfo,&numcommandsstored);//store the input line in the history variable
        loghistory(home,historyinfo,numcommandsstored);
        parse_and_execute_line(input_line);

        
    }
}