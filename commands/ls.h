#ifndef LS__H
#define LS__H
#include "../utils.h"
void lsla(char *filepath); //ls with -l and -a flags
void lsxa(char *filepath); //ls with only -a flag
void lslx(char *filepath); //ls with only -l flag
void lsxx(char *filepath); //ls with no flag
void ls(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int argnum);//main ls function which will be used in utils.c


#endif // !LS__H
