#include "echo.h"
#include <stdio.h>
#include <stdlib.h>
#include "../utils.h"
#include "../shell.h"
void echo(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int argnum)
{
    for(int i=1;i<argnum-1;i++)
    {
        printf("%s ",commandparams[i]);
    }
    printf("%s\n",commandparams[argnum-1]);
    return;
}