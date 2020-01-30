#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h> 
#include <fcntl.h>

int main(int argc, char** argv)
{
    if(argc!=2)
    {
        errx(1,"invalid amount of args");
    }
    char buffer[128+1];
    int filedes = open(argv[1],O_RDONLY);
    if(filedes==-1)
    {
        errx(errno, "invalid file :(");
    }
    int charsRead=0;
    do{
        charsRead=read(filedes,buffer,128);
        buffer[charsRead]='\0';
        printf("%s", buffer);
        fflush(stdout);
    }while(charsRead==128);
  
    close(filedes);
    return 0;
}