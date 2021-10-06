#include "fg.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
void fg(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int argnum)
{
    if(argnum != 2)
    {
        printf("incorrect number of arguments");
        return;
    }
    processlist p=PList->next;
    int counter=0;//stores the number of jobs
    for(;p;p=p->next)
    {
        counter++;
    }
    int jobno=myAtoi(commandparams[1]);
    if(jobno > counter)
    {
        printf("incorrect job number\n");
        return;
    }
    processlist pnew=PList->next;
    for(int i=0;i<counter-jobno;i++)
    {
        pnew=pnew->next;
    }
    // printf("%d", pnew->pid);
    int fgprocessid=pnew->pid;
    char fgprocessname[256];
    strcpy(fgprocessname,pnew->nameofprocess);
    if(kill(pnew->pid,18) < 0)
    {
        perror("invalid signo");
        exit;
    }       
    printf("sent %s with %d to foreground\n",fgprocessname,fgprocessid);
    int status;
    waitpid(fgprocessid,&status,WUNTRACED);
    deleteprocess(PList,fgprocessid);
    
    return;


}