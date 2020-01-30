#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    if(argc!=3)
    {
        errx(1,"invalid amount of args");
    }
    int bufferSize = 128;
    char buffer[bufferSize+1];
    int inFD = open(argv[1],O_RDONLY);
    int outFD=creat(argv[2], 0666);
    if(inFD==-1 || outFD==-1)
    {
        errx(errno, "invalid file :(");
    }
    int charsRead=0;
    while(((charsRead=read(inFD, buffer, bufferSize))>0))
    {
        buffer[bufferSize]='\0';
        write(outFD, buffer, charsRead);
        printf("%s", buffer);
    }

}