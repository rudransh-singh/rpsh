#include "fgbgprocess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <assert.h>
ptrtoprocessinfo makenode (int mypid, char myname[256])
{
    ptrtoprocessinfo p=(ptrtoprocessinfo)malloc(sizeof(processinfo));
    assert(p!=NULL);
    p->pid=mypid;
    strcpy(p->nameofprocess,myname);
    p->next=NULL;
    return p;
}

processlist createemptylist()
{
    ptrtoprocessinfo p=makenode(-1,"null");
    return p;
}

void insertprocess(processlist L, int mypid, char myname[100])
{
    ptrtoprocessinfo p=makenode(mypid,myname);
    p->next=L->next;
    L->next=p;
}

void deleteprocess(processlist L, int mypid)
{
    ptrtoprocessinfo checkpointer=L->next;
    ptrtoprocessinfo lagpointer=L;

    for(;checkpointer;)
    {
        if(checkpointer->pid==mypid)
        {
            lagpointer->next=checkpointer->next;
            ptrtoprocessinfo tempptr=checkpointer;
            checkpointer=checkpointer->next;
            free(tempptr);
        }
        else
        {
            checkpointer=checkpointer->next;
            lagpointer=lagpointer->next;
        }
    }
}

void searchforprocessname(processlist L, int mypid, char dest[256] )
{
    ptrtoprocessinfo p=L->next;
    for(;p;p=p->next)
    {
        if(p->pid==mypid)
        {
            strcpy(dest,p->nameofprocess);
            return;
        }
    }
}
void executebgprocess(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE], int argnum)
{
    char** args=(char **)malloc(sizeof(char*)*MAX_ARG_SIZE);
    for(int i=0;i<argnum-1;i++)
    {
        args[i]=commandparams[i];
    }
    args[argnum-1]=NULL;
    int pid=fork();

    if(pid < 0)//if forking was unsuccessful
    {
        perror(commandparams[0]);
    }
    else if(pid ==0)
    {
        setpgid(pid,pid);

        int x=execvp(args[0],args);
        if(x==-1)
        {
            printf("%s:Command not found\n",commandparams[0]);
        }
        
    }
    else 
    {
        setpgid(pid,pid); 
        printf("%s %d\n", commandparams[0],pid);
        insertprocess(PList,pid,commandparams[0]);
    }

}



void executefgprocess(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE], int argnum)
{
    char** args=(char **)malloc(sizeof(char*)*MAX_ARG_SIZE);
    for(int i=0;i<argnum;i++)
    {
        args[i]=commandparams[i];
    }
    args[argnum]=NULL;
    int pid=fork();
    if(pid < 0)//if forking was unsuccessful
    {
        perror(commandparams[0]);
    }
    else if(pid ==0)
    {
        setpgid(pid,pid);

        int x=execvp(args[0],args);
        if(x==-1)
        {
            printf("%s:Command not found\n",commandparams[0]);
        }
    }
    else 
    {
        signal(SIGTTOU, SIG_IGN);
        // tcsetpgrp(0,pid);
        int status=0;
        curfgpid=pid;
        strcpy(curfgpname,commandparams[0]);
        waitpid(pid,&status,WUNTRACED);
        // tcsetpgrp(0,getpgrp());
        signal(SIGTTOU, SIG_DFL);
        // printf("\n");
    }

}