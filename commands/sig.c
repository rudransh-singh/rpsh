#include "sig.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
void sig(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int argnum)
{
    if(argnum != 3)
    {
        printf("incorrect number of arguments\n");
        return;
    }
    int signo=myAtoi(commandparams[2]);
    if(signo < 1 || signo > 31)
    {
        printf("wrong signal number");
        return;
    }
    int jobno=myAtoi(commandparams[1]);
    processlist p=PList->next;
    int counter=0;//stores the number of jobs
    for(;p;p=p->next)
    {
        counter++;
    }
    if(jobno > counter)
    {
        printf("incorrect job no");
        return;
    }
    else
    {
        processlist pnew=PList->next;
        for(int i=0;i<counter-jobno;i++)
        {
            pnew=pnew->next;
        }
        // printf("%d", pnew->pid);
        if(kill(pnew->pid,signo) < 0)
        {
            perror("invalid signo");
            exit;
        }        

    }
    return;
}