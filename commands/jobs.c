#include "jobs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_JOBS_LIMIT 128
void makejoblistarray(jobslist jobs[],processlist P, int size)
{
    ptrtoprocessinfo p=PList->next;
    int counter=0;
    for(;p;p=p->next)
    {
        counter++;
    }
    ptrtoprocessinfo pointer=PList->next;//points to the first entry in the process list
    for(int i=0;i< size && pointer; i++,pointer=pointer->next)
    {
        strcpy(jobs[i].process.nameofprocess,pointer->nameofprocess);
        jobs[i].process.pid=pointer->pid;
        jobs[i].sno=counter-i;;
    }

    return;
}

int stringcomparator (const void *str1, const void *str2)
{
  jobslist *job1 = (jobslist *)str1;
  jobslist *job2 = (jobslist *)str2;
  return (strcmp(job1->process.nameofprocess,job2->process.nameofprocess));
}
void sortjobslist(jobslist jobs[], int size)
{
    qsort(jobs,size, sizeof(jobslist), stringcomparator);
}

char jobstatus(jobslist j)
{
    int processid=j.process.pid;
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


void jobsfn(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE], int argnum)
{
    int rflag=0;
    int sflag=0;
    if(argnum==1)// no arguments were passed
    {
        rflag=1;
        sflag=1;
    }
    else if(argnum ==2 && strcmp(commandparams[1],"-r")==0)
    {
        rflag=1;
        sflag=0;
    }
    else if (argnum == 2 && strcmp (commandparams[1], "-s")==0)
    {
        rflag=0;
        sflag=1;
    }
    else
    {
        printf("invalid number of arguments\n");
        return;
    }
    int counter=0;
    processlist pointer=PList->next;
    for(pointer;pointer;pointer=pointer->next)
    {
        counter++;
    }
    //counter will store the number of jobs currently in the background
    jobslist jobs[MAX_JOBS_LIMIT];
    makejoblistarray(jobs,PList,MAX_JOBS_LIMIT);
    qsort(jobs,counter, sizeof(jobslist), stringcomparator);
    for(int i=0;i<counter;i++)
    {
        char procstatus=jobstatus(jobs[i]);
        char finalprint[100]="";
        if(procstatus!='T')
        {
            strcpy(finalprint,"Running");
        }
        else if(procstatus=='T')
        {
            strcpy(finalprint,"Stopped");
        }
        if(rflag && sflag)
            printf("[%d] %s %s %d \n", jobs[i].sno,finalprint,jobs[i].process.nameofprocess,jobs[i].process.pid);
        else if(rflag && !sflag)
        {
            if (procstatus!='T')
                printf("[%d] %s %s %d \n", jobs[i].sno,finalprint,jobs[i].process.nameofprocess,jobs[i].process.pid);
        }
        else if (!rflag && sflag)
        {
            if (procstatus =='T')
                printf("[%d] %s %s %d \n", jobs[i].sno,finalprint,jobs[i].process.nameofprocess,jobs[i].process.pid);
        }
        
       

    }
    
    return ;
}