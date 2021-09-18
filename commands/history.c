#include "history.h"
#include <stdio.h>
#include <stdlib.h>
void history(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int argnum)
{

    if(argnum==1)
    {
        for(int i=0;i<numcommandsstored;i++)
        {
            printf("%s\n",historyinfo[i]);
        }
    }
    else if(argnum==2)
    {
        int number=myAtoi(commandparams[1]);
        for(int i=numcommandsstored-number;i<numcommandsstored;i++)
        {
            printf("%s\n",historyinfo[i]);
        }
    }
    return;
}