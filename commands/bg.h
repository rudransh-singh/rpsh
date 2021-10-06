#ifndef BG__H
#define BG__H
#include "../utils.h"
#include "fgbgprocess.h"
void bg(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE], int argnum);
char myjobstatus(int pid);

#endif