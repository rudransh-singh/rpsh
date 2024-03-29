#include <termios.h>
#include <ctype.h>
//Standard header files 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>
#include <signal.h>
#include <sys/wait.h>

//user-defined header files
#include "colors.h"
#include "display.h"
#include "commands/pwd.h"
#include "commands/cd.h"
#include "commands/echo.h"
#include "commands/fgbgprocess.h"
#include "commands/ls.h"
#include "commands/pinfo.h"
#include "commands/history.h"
#include "utils.h"

int max(int a,int b)
{
    return (a>b)?a:b;
}
char home[200];
processlist PList;
int curfgpid=0;
char curfgpname[MAX_ARG_SIZE];
int activejobs[129];
void handle_child_death(int sig)
{
    int stat;
    pid_t processpid;
    processpid=waitpid(-1, &stat, WNOHANG | WUNTRACED );
    if(processpid > 0)
    {
        char processname[256];
        searchforprocessname(PList,processpid,processname);
        // strcpy(processname,"muguop");
        if(WIFEXITED(stat))
        {
            printf("%s with pid= %d exited normally\n",processname,processpid);
            deleteprocess(PList,processpid);
        }
        else if (WIFSTOPPED(stat)) {
            printf("%s with pid %d suspended normally\n",processname, processpid);
        }
        else
        {
            printf("%s with pid= %d exited abnormally\n",processname,processpid);
            deleteprocess(PList,processpid);
        }
        display(home);
        fflush(stdout);
    }
}

int  openhistoryfile(char history[20][MAX_ARG_SIZE])
{
    FILE* fh;
    fh=fopen("history.txt","r");
    char* temp;//temporary pointer for strtok_r;
    char* token;
    char *buffer;
    size_t buffersize=10240;
    buffer=(char *)malloc(buffersize*sizeof(char));
    getline(&buffer,&buffersize,fh);
    //now buffer stores the contents of the history file
    token=strtok_r(buffer,";",&temp);
    int count=0;
    while(token!=NULL)
    {
        strcpy(history[count],token);
        token=strtok_r(NULL,";",&temp);
        count++;
    }
    fclose(fh);
    return count;
}
//history[0]==oldest instruction, history[19]==newest instruction
void storeinhistory(char inputline[], char history[20][MAX_ARG_SIZE], int *numcommandsstored)
{
    int index= *numcommandsstored;
    if(strcmp(history[index-1],inputline)==0)
        return; 
    if(*(numcommandsstored)==20)
    {
        for(int i=0;i<19;i++)
        {
            strcpy(history[i],history[i+1]);
        }
        strcpy(history[19],inputline);
    }
    else
    {
        strcpy(history[index],inputline);
        (*numcommandsstored)++;
    }
}

void loghistory(char home[200],char history[20][MAX_ARG_SIZE],int numcommandsstored)
{
    char filepath[512];
    strcpy(filepath,home);
    strcat(filepath,"/history.txt");
    FILE* fh;
    fh=fopen(filepath,"w");
    for(int i=0;i<numcommandsstored;i++)
    {
        fprintf(fh,"%s;",history[i]);
    }
    fclose(fh);
}

char historyinfo[20][MAX_ARG_SIZE];// makes a 2d array to store previous commands
int numcommandsstored;


void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/**
 * stdout and stdin are buffered we disable buffering on that
 * After entering in raw mode we read characters one by one
 * Up arrow keys and down arrow keys are represented by 3 byte escape codes
 * starting with ascii number 27 i.e. ESC key
 * This way we interpret arrow keys
 * Tabs are usually handled by the term, but here we are simulating tabs for the sake of simplicity
 * Backspace move the cursor one control character to the left
 * @return
 */



void takeinput(char inp[])
{
    char c;
        setbuf(stdout, NULL);
        enableRawMode();
        memset(inp, '\0', 256);
        int pt = 0;
        int count=numcommandsstored;
        while (read(STDIN_FILENO, &c, 1) == 1) {
            if (iscntrl(c)) {
                if (c == 10) {
                    count=numcommandsstored;
                    break;
                }
                else if (c == 27) {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2) { // length of escape code
                        {
                            if(strcmp(buf,"[A")==0)
                            {
                                printf("\r");
                                display(home);
                                for(int i = 0; i < pt+1; i++)
                                {
                                    printf(" ");
                                }
                                count--;
                                count=max(count,0);
                                printf("\r");
                                display(home);
                                strcpy(inp,historyinfo[count]);
                                pt=strlen(inp);
                                printf("%s",historyinfo[count]);
                            }
                        }
                    }
                } else if (c == 127) { // backspace
                    if (pt > 0) {
                        if (inp[pt-1] == 9) {
                            for (int i = 0; i < 7; i++) {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        printf("\b \b");
                    }
                } else if (c == 9) { // TAB character
                    inp[pt++] = c;
                    for (int i = 0; i < 8; i++) { // TABS should be 8 spaces
                        printf(" ");
                    }
                } else if (c == 4) {
                    exit(0);
                } else {
                    printf("%d\n", c);
                }
            } else {
                inp[pt++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();

}
void handle_ctrlc(int sig)
{
    if(curfgpid==-1)
    {
        return;
    }
    kill(curfgpid,SIGINT);
    printf("\n");
    // display(home);
    curfgpid=-1;
    strcpy(curfgpname,"");
    return;
}

void handle_ctrlz(int sig)
{
   if(curfgpid == -1)
   {
       return; 
   }
   kill(curfgpid,SIGTSTP);
   insertprocess(PList,curfgpid,curfgpname);
   printf("\n");
   curfgpid=-1;
   strcpy(curfgpname,"");
   return;
}
int main() {
    for(int i=0;i<129;i++)
        activejobs[i]=0;
    getcwd(home,200);
    signal(SIGINT,handle_ctrlc);
    signal(SIGTSTP,handle_ctrlz);
    numcommandsstored= openhistoryfile(historyinfo); // function to open history.txt and store it's contents in history array
    //num commands stored contains the number of commands currently stored in history
    PList=createemptylist();

    signal(SIGCHLD,handle_child_death);
 while(1)
    {
        char input_line[256]; // stores the input line (which is taken through readline function)
        char** commandparams; //will store the command info (command name and params) after parsing
        curfgpid=-1;
        strcpy(curfgpname,"");


        display(home); //displays the prompt
        takeinput(input_line);
        printf("\n");
        storeinhistory(input_line,historyinfo,&numcommandsstored);//store the input line in the history variable
        loghistory(home,historyinfo,numcommandsstored);
        parse_and_execute_line(input_line);

        
    }

    return 0;
}
