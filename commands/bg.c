#include "bg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
char myjobstatus(int pid)
{
    int processid=pid;
    char filepath[512]="";// this will store the file path /proc/${pid}/stat
    strcpy(filepath,"/proc/");
  
    //now store the pid in a string
    int length=snprintf(NULL,0,"%d",processid);
    char pidstring[length+1];
    snprintf(pidstring,length+1,"%d",processid);

    //now pidstring contains process id of the process in string format

    strcat(filepath,pidstring);
    strcat(filepath,"/stat");

    //now filepath is "/proc/pid/stat"
    FILE *fp;
    fp=fopen(filepath,"r");
    if(fp==NULL)
    {
        printf("Process does not exist\n");
        return -1;
    }
    char *buffer;
    size_t buffersize=10240;
    buffer=(char *)malloc(buffersize*sizeof(char));
    getline(&buffer,&buffersize,fp);
    char* mytoken;
    int count=0;
    char* temp;
    char pinfoparams[MAX_ARG_NUM][MAX_ARG_SIZE];
    mytoken=strtok_r(buffer," ",&temp);
    strcpy(pinfoparams[count],mytoken);
    while(mytoken != NULL)
    {
        mytoken=strtok_r(NULL," \t",&temp);
        count++;
        if(mytoken!=NULL)
        strcpy(pinfoparams[count],mytoken);
    }
    return pinfoparams[2][0];

}
void bg(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int argnum)
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
    char jobstat=myjobstatus(fgprocessid);
    if(jobstat != 'T')
    {
        printf("already running\n");
        return;
    }
    if(kill(pnew->pid,18) < 0)
    {
        perror("invalid signo");
        exit;
    }   
    printf("sent %s with %d to running\n",fgprocessname,fgprocessid);
    return;
}