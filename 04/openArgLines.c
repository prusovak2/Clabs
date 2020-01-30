#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <poll.h>


int main(int argc, char** argv)
{
    if(argc!=2)
    {
        errx(1,"invalid amount of args");
    }
    char buffer[1+1];
    int filedes = open(argv[1],O_RDONLY);
    if(filedes==-1)
    {
        errx(errno, "invalid file :(");
    }
    int charsRead=1;
    int charsOnLine =0;
    while((charsRead==1))
    {
            charsRead=read(filedes,buffer,1);
            buffer[charsRead]='\0';
            printf("%s", buffer);
            fflush(stdout);
            charsOnLine++;
            if(buffer[0]=='\n')
            {
                charsOnLine=0;
            }
            if(charsOnLine==128)
            {
                printf("\n");
                charsOnLine=0;
            }
        

    }
//read po charu, rovnou print

    close(filedes);
    return 0;
}