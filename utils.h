#ifndef UTILS__H
#define UTILS__H
#define MAX_ARG_NUM 256 //max number of arguments is 256
#define MAX_ARG_SIZE 256 //max number of arguments is 256
#define MAX_PIPE_LIMIT 100 // max number of commands that can be piped together
int myAtoi(char* str); //to convert string to int
void read_line(char input_line[]);
void setangularflags(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int argnum ,int* write, int* append, int* read);
void deletefromarray(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int* argnum, int pos);
void parse_and_execute_line(char input_line[]);
void execute_line(char  commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int count);
void sethomedirectory(char directory[],char homedirectory[]);
int checkpipes(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int argnum);//goes through a token and sees whether there are any pipe symbols
void copytillpipe( char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int argnum, char pipecommand[MAX_ARG_NUM][MAX_ARG_SIZE], int* pos);
//copies command till the pipe so if command is cat *.txt | grep hello then command will contain cat *.txt only
void execute_pipe(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE], int count);
#endif // !UTILS__H