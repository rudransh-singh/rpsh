#ifndef HISTORY__H
#include "../utils.h"
extern char historyinfo[20][MAX_ARG_SIZE];// makes a 2d array to store previous commands
extern int numcommandsstored;
void history(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int argnum);//shows the history of the terminal, that is, a list of last 20 commands executed


#endif // !HISTORY__H