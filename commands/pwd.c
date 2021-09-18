#include <stdio.h>
#include <unistd.h>
#include "pwd.h"
void pwd()
{
    char curr_directory[200];
    getcwd(curr_directory,sizeof(curr_directory));
    printf("%s\n",curr_directory);
}