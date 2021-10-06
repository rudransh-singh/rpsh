#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <ctype.h>
#include "utils.h"
#include "shell.h"
#include "commands/pwd.h"
#include "commands/cd.h"
#include "commands/echo.h"
#include "commands/ls.h"
#include "commands/fgbgprocess.h"
#include "commands/pinfo.h"
#include "commands/history.h"
#include "commands/jobs.h"
#include "commands/sig.h"
#include "commands/fg.h"
#include "commands/bg.h"
int stdoutfileno;
int stdinfileno;

void sethomedirectory(char directory[],char homedirectory[])
{
        int i=0;
    for(i=0;i<strlen(homedirectory)&&directory[i];i++)
    {
        if(directory[i]!=homedirectory[i])
            break;
    }
        char finalpath[200];
    if(i==strlen(homedirectory))
    {
        finalpath[0]='~';
        finalpath[1]='\0';
        strcat(finalpath,directory+i);
    }
    else
    {
        strcpy(finalpath,directory);
    }

    strcpy(directory,finalpath);

}



int myAtoi(char* str)
{
    int res = 0;
    for (int i = 0; str[i] != '\0'; ++i)
    {
        if(!isdigit(str[i]))
        {
            printf("argument is not a number\n");
            return -1;
        }
        else
        {
        res = res * 10 + str[i] - '0';
        }        

    }
 
    return res;
}
 
void read_line(char input_line[])//this function reads the line given as input
{
    fgets(input_line,1024,stdin); // using getline since it has \n as delimiter
    return ;
}
void parse_and_execute_line(char input_line[])
{
   stdoutfileno=dup(STDOUT_FILENO);
   stdinfileno=dup(STDIN_FILENO);
   const char s[4] = ";\n";
   char *token;
   char* tmp;
   char* anothertmp;
   /* get the first token */
   token = strtok_r(input_line, s,&tmp);  
   /* walk through other tokens */
   while( token != NULL ) {
        char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE];
        int count=0;//will count the number of arguments
        char *tokeninside;
        tokeninside=strtok_r(token," \t",&anothertmp);
        strcpy(commandparams[count],tokeninside);
        count++;
        tokeninside=strtok_r(NULL," \t",&anothertmp);
        while(tokeninside != NULL)
        {
            strcpy(commandparams[count],tokeninside);
            count++;
            tokeninside=strtok_r(NULL," \t",&anothertmp);
        }
        execute_pipe(commandparams,count);
      token = strtok_r(NULL, s,&tmp);
   }
if(dup2(stdoutfileno,STDOUT_FILENO)<0)
{
    perror("stdout dup failed");
    exit;
}
if(dup2(stdinfileno,STDIN_FILENO)<0)
{
    perror("stdin dup failed");
    exit;
}
close(stdinfileno);
close(stdoutfileno);
   return;
}
/*
The function goes through all the params, and checks for input /output redirection.
If it sees >, changes stdout to the file pointer pointing to the file.
If it sees >>, changes stdout to the file pointer pointing to the file in append mode.
If it sees <. changes stdin to the file pointer pointing to the file.
*/
void deletefromarray(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int* argnum, int pos)
{
    for(int i=pos;i<(*argnum)-1;i++)
    {
        strcpy(commandparams[i],commandparams[i+1]);
    }
    (*argnum)--;
}

int checkpipes(char commandparamsp[MAX_ARG_NUM][MAX_ARG_SIZE],int argnum)
{
    int pipecount=0;
    for(int i=0;i< argnum; i++)
    {
        if(strcmp(commandparamsp[i],"|")==0)
        {
            pipecount++;
        }
    }
    return pipecount;
}

void copytillpipe( char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE], int argnum, char pipecommand[MAX_ARG_NUM][MAX_ARG_SIZE], int *pos)
{
    int j=0;
    for( int i= *pos; i < argnum && strcmp(commandparams[i],"|")!=0 ;i++,(*pos)++,j++)
    {
        strcpy(pipecommand[j],commandparams[i]);
    }
    (*pos)++;
    return;
}


void execute_pipe(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE], int count)
{
    
int pipecount=checkpipes(commandparams, count);
if(pipecount==0)
{
    execute_line(commandparams,count);
}
else
{
    char pipedcommands[MAX_PIPE_LIMIT][MAX_ARG_NUM][MAX_ARG_SIZE];
    int pipedargno[MAX_PIPE_LIMIT];//stores the number of arguments in each piped command
    //piped commands will store all the piped commands systematically
    int counter=0;
    int j=0;
    for(int i=0;i < count ; i++)
    {
        if(strcmp(commandparams[i],"|")!=0)
        {
            strcpy(pipedcommands[counter][j],commandparams[i]);
            j++;
        }
        else
        {
            pipedargno[counter]=j;
            counter++;
            
            j=0;
        }
    }
    pipedargno[counter]=j;

    /*
    say the command was command 1 | command 2 | command 3 | command 4
    then pipecommands[0]=command 1
    pipecomamnds[1]=comamnd 2
    pipedcommands[2]=command 3
    pipedcommands[3]=command 4
    */
    // int pipesfd[pipecount*2];
    // for(int i=0;i< pipecount ;i++)
    // {
    //     pipe(pipesfd +2*i);
    // }
    int commandcounter=0;
    while(commandcounter != pipecount)
    {
        int pipesfd[2];
        pipe(pipesfd);
        dup2(pipesfd[1],STDOUT_FILENO);
        close(pipesfd[1]);
        execute_line(pipedcommands[commandcounter],pipedargno[commandcounter]);
        dup2(pipesfd[0],STDIN_FILENO);
        close(pipesfd[0]);
        commandcounter++;
    }
    dup2(stdoutfileno,STDOUT_FILENO);
    execute_line(pipedcommands[commandcounter],pipedargno[commandcounter]);


    // int pipesfd[2*pipecount];
    // for(int i=0;i<pipecount;i++)
    // {
    //     pipe(pipesfd + 2*i);
    // }
    // int commandc=0;
    // int status;
    // while(commandc != pipecount +1)
    // {
    //     int pid=fork();
    //     if (pid < 0)
    //     {
    //         perror("forking not succesful");
    //     }
    //     else if(pid==0)
    //     {
    //         if(commandc!=0)
    //         {
    //             if(dup2(pipesfd [ (commandc -1 )* 2],STDIN_FILENO) < 0)
    //             {
    //                 perror("dup  was not succesful");
    //             } 
    //         }
    //         if(commandc != pipecount)
    //         {
    //             if(dup2(pipesfd [ commandc*2 + 1 ], STDOUT_FILENO) < 0)
    //             {
    //                 perror("dup not succesful");
    //             } 


    //         }
    //         for(int i=0;i< 2 *pipecount;i++)
    //         {
    //             close(pipesfd[i]);
    //         }
    //         execute_line(pipedcommands[commandc],pipedargno[commandc]);
    //         commandc++;
    //     }
    //     else 
    //     {
    //         for(int i=0; i< 2 *pipecount ; i++)
    //         {
    //             close(pipesfd[i]);
    //         }
    //        for(int i=0; i < pipecount +1 ;i++)
    //        {
    //            wait(0);
    //        }
    //     }
    // }




}
}


void execute_line(char  commandparams[MAX_ARG_NUM][MAX_ARG_SIZE], int count)
{
    int fr,fw,fa;
    for(int i=0;i< count; i++)
    {
        if(strcmp(commandparams[i],"<")==0)
        {
            fr=open(commandparams[i+1], O_RDONLY ,0644);
            if(fr < 0)
            {
                perror("Failed to open file");
                return;
            }

            if(dup2(fr,STDIN_FILENO)<0)
            {
                perror("Unable to duplicate file descriptor");
                return;
            }
            close(fr);
            deletefromarray(commandparams,&count,i);
        }
        if(strcmp(commandparams[i],">")==0)
        {
            fw=open(commandparams[i+1], O_WRONLY | O_CREAT | O_TRUNC ,0644);
            if(fw < 0)
            {
                perror("Failed to open file");
                return;
            }
            if(dup2(fw,STDOUT_FILENO)<0)
            {
                perror("Unable to duplicate file descriptor");
                return;
            }
            close(fw);
            deletefromarray(commandparams,&count,i);
            deletefromarray(commandparams, &count, i);
        }
        if(strcmp(commandparams[i],">>")==0)
        {
            fw=open(commandparams[i+1], O_WRONLY | O_CREAT | O_APPEND,0644);
            if(fw < 0)
            {
                perror("Failed to open file");
                return;
            }
            if(dup2(fw,STDOUT_FILENO)<0)
            {
                perror("Unable to duplicate file descriptor");
                return;
            }
            close(fw);
            deletefromarray(commandparams,&count,i);
            deletefromarray(commandparams, &count, i);
        }

    }



    if(strcmp(commandparams[0],"pwd")==0)
    {
        if(count>1)
            printf("pwd:too many arguments\n");
        else
            pwd();
    }
    else if(strcmp(commandparams[0],"cd")==0)
    {
        cd(commandparams,count);       
    }
    else if(strcmp(commandparams[0],"echo")==0)
    {
        echo(commandparams,count);
    }
    else if(strcmp(commandparams[0],"ls")==0)
    {
        ls(commandparams,count);
    }
    else if(strcmp(commandparams[0],"pinfo")==0)
    {
        pinfo(commandparams,count);
    }
    else if(strcmp(commandparams[0],"repeat")==0)
    {
        char temp[MAX_ARG_NUM][MAX_ARG_SIZE];
        for(int i=2;i<count;i++)
        {
            strcpy(temp[i-2],commandparams[i]);
        }
        for(int i=0;i<myAtoi(commandparams[1]);i++)
        execute_line(temp,count-2);
    }
    else if(strcmp(commandparams[0],"history")==0)
    {
        history(commandparams,count);
    }
    else if(strcmp(commandparams[0],"quit")==0)
    {
        exit(0);
    }
    else if(strcmp(commandparams[0],"jobs")==0)
    {
        jobsfn(commandparams,count);
    }
    else if(strcmp(commandparams[0],"sig")==0)
    {
        sig(commandparams,count);
    }
    else if (strcmp(commandparams[0],"fg")==0)
    {
        fg(commandparams,count);
    }
    else if (strcmp(commandparams[0],"bg")==0)
    {
        bg(commandparams,count);
    }
    else
    {
        if(strcmp(commandparams[count-1],"&")==0)
        {
            executebgprocess(commandparams,count);
        }
        else
        {
            // printf("hello world");
             executefgprocess(commandparams,count);
        }
    }
    
    // dup2(froriginal,STDIN_FILENO);
    // dup2(fworiginal,STDOUT_FILENO);
}