#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "shell.h"
#include "commands/pwd.h"
#include "commands/cd.h"
#include "commands/echo.h"
#include "commands/ls.h"
#include "commands/fgbgprocess.h"
#include "commands/pinfo.h"
#include "commands/history.h"

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
        res = res * 10 + str[i] - '0';
 
    return res;
}
 
void read_line(char input_line[])//this function reads the line given as input
{
    fgets(input_line,1024,stdin); // using getline since it has \n as delimiter
    return ;
}
void parse_and_execute_line(char input_line[])
{
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
        execute_line(commandparams,count);
      token = strtok_r(NULL, s,&tmp);
   }
   
   return;
}
void execute_line(char  commandparams[MAX_ARG_NUM][MAX_ARG_SIZE], int count)
{
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
}