#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

int main(int argc, char** argv)
{
    if(argc!=2)
    {
        errx(1,"invalid amount of args");
    }
    int bufferSize = 1;
    char FrontBuffer[bufferSize+1];
    char BackBuffer [bufferSize+1];
    int frontFD = open(argv[1],O_RDONLY);
    int backFD = open(argv[1],O_RDONLY );
    if(frontFD==-1 || backFD==-1)
    {
        errx(errno, "invalid file :(");
    }
    off_t backIndex = lseek(backFD, -2, SEEK_END);
    int counter =0;
    int half =((int)backIndex)/2;
    while(counter<=half)
    {
        int b = read(backFD, BackBuffer, bufferSize);
        int f = read(frontFD, FrontBuffer, bufferSize); 
        if(b!=1 || f!=1)
        {
            errx(errno, "something went wrong :(");
        }
        FrontBuffer[bufferSize]='\0';
        BackBuffer[bufferSize]='\0';

        if(FrontBuffer[0]!=BackBuffer[0])
        {
            printf("not a palindrom");
            return 1;
        }

        counter++;
        int pom = ((int)backIndex - counter); 
        lseek(backFD,pom, SEEK_SET);
        //printf("%s%s ", FrontBuffer, BackBuffer);
        //fflush(stdout);
    }
    printf("It is a palindrom!");
    return 0;
}