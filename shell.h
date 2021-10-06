#ifndef SHELL__H
#define SHELL__H
extern char home[200];
int  openhistoryfile(char history[20][MAX_ARG_SIZE]);
void handle_child_death(int sig);
void handle_ctrlc(int sig);
void handle_ctrlz(int sig);
void storeinhistory(char input_line[],char history[20][MAX_ARG_SIZE], int *x);
void loghistory(char homedirectory[200],char history[20][MAX_ARG_SIZE], int numcommmandsstored);
#endif // !SHELL__H
