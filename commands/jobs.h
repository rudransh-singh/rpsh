#ifndef JOBS__H
#define JOBS__H
#include  "fgbgprocess.h"
#include "../utils.h"
typedef struct jobslist jobslist;

struct jobslist //maintaining a jobs list to deal with the sorting and stuff 
{
    processinfo process;
    int sno;
};

void makejoblistarray(jobslist jobs[],processlist P, int size);
void sortjobslist(jobslist jobs[], int size);
void jobsfn(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE], int argnum);
int stringcomparator (const void *str1, const void *str2);
char jobstatus(jobslist j);
#endif