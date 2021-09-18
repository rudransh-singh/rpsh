#include "../utils.h"
#include "../shell.h"
#include <stdio.h>
#include <stdlib.h>
#include "pinfo.h"
#include <string.h>
#include <unistd.h>
void pinfo(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int argnum)
{
    int shellpid=getpid();// shell pid now stores the pid of the shell
    FILE* fp;
    int processid=0;// process id of the process i want to get pinfo on
    if (argnum==1)
    {
        processid=shellpid;// if argnum==1 then take pid of shell
    }
    else
    {
        processid=myAtoi(commandparams[1]);// else take pid of the input param
    }
    char filepath[512]="";// this will store the file path /proc/${pid}/stat
    strcpy(filepath,"/proc/");
  
    //now store the pid in a string
    int length=snprintf(NULL,0,"%d",processid);
    char pidstring[length+1];
    snprintf(pidstring,length+1,"%d",processid);

    //now pidstring contains process id of the process in string format

    strcat(filepath,pidstring);
    strcat(filepath,"/stat");
    

    //now open the file
    fp=fopen(filepath,"r");
    if(fp==NULL)
    {
        printf("Process does not exist\n");
        return;
    }
    char *buffer;
    size_t buffersize=10240;
    buffer=(char *)malloc(buffersize*sizeof(char));
    getline(&buffer,&buffersize,fp);
    //now buffer stores the stat info of the file;
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
    printf("pid--%s\n",pinfoparams[0]);
    char foregroundflag;
    if(strcmp(pinfoparams[4],pinfoparams[7])==0)
        foregroundflag='+';
    printf("Process Status--%s%c\n",pinfoparams[2],foregroundflag);
    printf("memory--%s\n",pinfoparams[22]);
    printf("Executable Path--");
    char executablepath[1024];
    strcpy(filepath,"/proc/");
    strcat(filepath,pidstring);
    strcat(filepath,"/exe");
    ssize_t len=readlink(filepath,executablepath,sizeof(executablepath));
    if(len==-1)
    {
        strcpy(executablepath,"NULL");
        executablepath[4]='\0';
    }
    else
        executablepath[len]='\0';
    sethomedirectory(executablepath,home);
    printf("%s\n",executablepath);
    free(buffer);

}
