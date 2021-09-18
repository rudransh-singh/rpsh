#ifndef UTILS__H
#define MAX_ARG_NUM 256 //max number of arguments is 256
#define MAX_ARG_SIZE 256 //max number of arguments is 256
int myAtoi(char* str); //to convert string to int
void read_line(char input_line[]);

void parse_and_execute_line(char input_line[]);
void execute_line(char  commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int count);
void sethomedirectory(char directory[],char homedirectory[]);
#endif // !UTILS__H