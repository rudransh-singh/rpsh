#include <stdio.h>
#include "ls.h"
#include "../utils.h"
#include "../shell.h"
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <dirent.h> //using this for DIR data type 
#include <sys/stat.h> //for using stat struct
#include <string.h>
#include <grp.h>
#include <time.h>
#define ll long long
void lsxx(char *filepath)
{
    DIR *directory;
    struct dirent* d;

    //open the directory 
    directory= opendir(filepath); //open directory stream
    if(directory==NULL)
    {
        perror(filepath);
        return;
    }

    while((d= readdir(directory))!=NULL)//d keeps storing the pointer to struct containing info on next directory
    {
        char *filename=d->d_name;
        if(filename[0]!='.')
            printf("%s\n",filename);       
    }
    printf("\n");
}
void lsxa(char *filepath)
{
    DIR *directory;
    struct dirent* d;

    //open the directory 
    directory= opendir(filepath); //open directory stream
    if(directory==NULL)
    {
        perror(filepath);
        return;
    }

    while((d= readdir(directory))!=NULL)//d keeps storing the pointer to struct containing info on next directory
    {
            printf("%s\n",d->d_name);       
    }
    printf("\n");

}

void lslx(char *filepath)
{
    DIR *directory;
    struct dirent* d;// for moving through all the files
    struct stat s;//for displaying stats
    struct tm* t;// for displaying time
    char date[100];
    
    directory=opendir(filepath);
    if(directory==NULL)
    {
        perror(filepath);
        return;
    }
    
    struct dirent* dtotal;//this will be used for calculating the total
    struct stat stotal;
    DIR *directorytotal;
    directorytotal=opendir(filepath);
    if(directorytotal==NULL)
    {
        perror(filepath);
        return;
    }
    ll total=0;// will store the total, which sum of st_blocks of each file
    while((dtotal=readdir(directorytotal))!=NULL)
    {
        char *filename=dtotal->d_name;
        char pathtofilesinside[1024];
        strcpy(pathtofilesinside,filepath);
        strcat(pathtofilesinside,"/");
        strcat(pathtofilesinside,filename);
        
        lstat(pathtofilesinside,&s);
        if (filename[0]!='.')
        {
            total+=s.st_blocks;
        }

    }
    printf("total:%lld\n",total/2);

    while((d=readdir(directory))!=NULL)
    {
        char *filename=d->d_name;

        char pathtofilesinside[1024];
        strcpy(pathtofilesinside,filepath);
        strcat(pathtofilesinside,"/");
        strcat(pathtofilesinside,filename);

        // now pathtofilesinside stores the complete file path to the file inside the directory

        lstat(pathtofilesinside,&s);
        //s now stores stat info of the file

        if(filename[0]!='.')
        {
            char filetype = '-';

            if(__S_ISTYPE((s.st_mode), __S_IFDIR))
            {
                filetype = 'd';
            }

            else if(__S_ISTYPE((s.st_mode), __S_IFCHR))
            {
                filetype = 'c';
            }

            else if(__S_ISTYPE((s.st_mode), __S_IFBLK))
            {
                filetype = 'b';
            }

            else if(__S_ISTYPE((s.st_mode), __S_IFREG))
            {
                filetype = '-';
            }

            else if(__S_ISTYPE((s.st_mode), __S_IFLNK))
            {
                filetype = 'l';
            }

            else if(__S_ISTYPE((s.st_mode), __S_IFSOCK))
            {
                filetype = 's';
            }

            else if(__S_ISTYPE((s.st_mode), __S_IFIFO))
            {
                filetype = 'f';
            }

            printf("%c",filetype);

            printf((s.st_mode & S_IRUSR) ? "r" : "-");
            printf((s.st_mode & S_IWUSR) ? "w" : "-");
            printf((s.st_mode & S_IXUSR) ? "x" : "-");
            printf((s.st_mode & S_IRGRP) ? "r" : "-");
            printf((s.st_mode & S_IWGRP) ? "w" : "-");
            printf((s.st_mode & S_IXGRP) ? "x" : "-");
            printf((s.st_mode & S_IROTH) ? "r" : "-");
            printf((s.st_mode & S_IWOTH) ? "w" : "-");
            printf((s.st_mode & S_IXOTH) ? "x" : "-");
            printf(" %3d ", (int)s.st_nlink);
            printf("%s ", getpwuid(s.st_uid)->pw_name);
            printf("%s ", getgrgid(s.st_gid)->gr_name);
            printf(" %6.ld ",s.st_size);
            t = localtime(&s.st_mtime);
            strftime(date, 20, "%b %d %H:%M", localtime(&(s.st_mtime)));
             printf("%s ", date);
             printf("%s\n",filename);

        }
    }
    printf("\n");
    
}
void lsla(char *filepath)
{
    DIR *directory;
    struct dirent* d;// for moving through all the files
    struct stat s;//for displaying stats
    struct tm* t;// for displaying time
    char date[100];

    directory=opendir(filepath);
    if(directory==NULL)
    {
        perror(filepath);
        return;
    }

    struct dirent* dtotal;//this will be used for calculating the total
    struct stat stotal;
    DIR *directorytotal;
    directorytotal=opendir(filepath);
    if(directorytotal==NULL)
    {
        perror(filepath);
        return;
    }
    ll total=0;// will store the total, which sum of st_blocks of each file
    while((dtotal=readdir(directorytotal))!=NULL)
    {
        char *filename=dtotal->d_name;
        char pathtofilesinside[1024];
        strcpy(pathtofilesinside,filepath);
        strcat(pathtofilesinside,"/");
        strcat(pathtofilesinside,filename);
        
        lstat(pathtofilesinside,&s);
            total+=s.st_blocks;
    }
    printf("total:%lld\n",total/2);
    while((d=readdir(directory))!=NULL)
    {
        char *filename=d->d_name;

        char pathtofilesinside[1024];
        strcpy(pathtofilesinside,filepath);
        strcat(pathtofilesinside,"/");
        strcat(pathtofilesinside,filename);

        // now pathtofilesinside stores the complete file path to the file inside the directory

        lstat(pathtofilesinside,&s);
        //s now stores stat info of the file

        
        
            char filetype = '-';

            if(__S_ISTYPE((s.st_mode), __S_IFDIR))
            {
                filetype = 'd';
            }

            else if(__S_ISTYPE((s.st_mode), __S_IFCHR))
            {
                filetype = 'c';
            }

            else if(__S_ISTYPE((s.st_mode), __S_IFBLK))
            {
                filetype = 'b';
            }

            else if(__S_ISTYPE((s.st_mode), __S_IFREG))
            {
                filetype = '-';
            }

            else if(__S_ISTYPE((s.st_mode), __S_IFLNK))
            {
                filetype = 'l';
            }

            else if(__S_ISTYPE((s.st_mode), __S_IFSOCK))
            {
                filetype = 's';
            }

            else if(__S_ISTYPE((s.st_mode), __S_IFIFO))
            {
                filetype = 'f';
            }


            printf("%c",filetype);

            printf((s.st_mode & S_IRUSR) ? "r" : "-");
            printf((s.st_mode & S_IWUSR) ? "w" : "-");
            printf((s.st_mode & S_IXUSR) ? "x" : "-");
            printf((s.st_mode & S_IRGRP) ? "r" : "-");
            printf((s.st_mode & S_IWGRP) ? "w" : "-");
            printf((s.st_mode & S_IXGRP) ? "x" : "-");
            printf((s.st_mode & S_IROTH) ? "r" : "-");
            printf((s.st_mode & S_IWOTH) ? "w" : "-");
            printf((s.st_mode & S_IXOTH) ? "x" : "-");
            printf(" %3d ", (int)s.st_nlink);
            printf("%s ", getpwuid(s.st_uid)->pw_name);
            printf("%s ", getgrgid(s.st_gid)->gr_name);
            printf("%6.ld ",s.st_size);
            t = localtime(&s.st_mtime);
            strftime(date, 20, "%b %d %H:%M", localtime(&(s.st_mtime)));
             printf("%s ", date);
             printf("%s\n",filename);
    }
    printf("\n");

}


void ls(char commandparams[MAX_ARG_NUM][MAX_ARG_SIZE],int argnum)
{
    // we will first check for the flags, by going through all the arguments and then go through the files
    int aflag=0; // will become 1 if -a is encountered
    int lflag=0; // will become 1 if -l is encountered
    int argflag=0;//checks whether we have any directories as input
    for(int i=1;i<argnum;i++)
    {
        if(strcmp(commandparams[i],"-la")==0 || strcmp(commandparams[i],"-al")==0)
        {
            aflag=1;
            lflag=1;
            break;
        }
        if(strcmp(commandparams[i],"-l")==0)
        {
            lflag=1;
        }
        if(strcmp(commandparams[i],"-a")==0)
        {
            aflag=1;
        }
        if(commandparams[i][0]!='-')
        {
            argflag++;
        }
    }
    //now that we have checked the flags
    for(int i=1;i<argnum;i++)
    {
        if(commandparams[i][0]!='-')
        {
            struct stat s;
            lstat(commandparams[i],&s);
            if(__S_ISTYPE((s.st_mode), __S_IFDIR)==0)
            {
                printf("%s\n",commandparams[i]);
            }
            else
            {
            if(argflag>1)
                printf("%s:\n",commandparams[i]);
            if(aflag && lflag)
            lsla(commandparams[i]);
            else if(aflag && !lflag)
            lsxa(commandparams[i]);
            else if(!aflag && lflag)
            lslx(commandparams[i]);
            else
            lsxx(commandparams[i]);
            }
        }

    }
        if(!argflag)
        {
            if(aflag && lflag)
            lsla(".");
            else if(aflag && !lflag)
            lsxa(".");
            else if(!aflag && lflag)
            lslx(".");
            else
            lsxx(".");
        }
}
