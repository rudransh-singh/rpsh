#ifndef FGBGPROCESS__H
#include "../utils.h"
void executefgprocess(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int argnum);
void executebgprocess(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int argnum);
typedef struct processinfo processinfo;
typedef processinfo* ptrtoprocessinfo;
typedef ptrtoprocessinfo processlist;

struct processinfo
{
    int pid;
    char nameofprocess[256];
    processinfo* next;
};//this stores the info about the process, mainly it's name and the process id, to be used when dealing with SIGCHLD signals
extern processlist PList;

ptrtoprocessinfo makenode (int mypid, char myname[256]); //makes a node with pid as mypid and nameofprocess as myname
processlist createemptylist();//creates an empty list

void insertprocess(processlist L,int mypid, char myname[256]);//inserts a node with given pid and name
void deleteprocess(processlist L,int mypid);//deletes a node with given pid(pids are unique for all processes)
void searchforprocessname(processlist L, int mypid, char dest[256] );// searches for name of given pid and stores it in dest[]


#endif // !FGBGPROCESS__H