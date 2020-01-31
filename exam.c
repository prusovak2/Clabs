#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include "debugPrint.h"
#include "safeAlloc.h"

#define MAX_FILENAME_LEN 255
#define CHAR_BUFFER_SIZE  1024
#define LINE_BUFFER_SIZE  4096

char *  ReadLineFromFile(int * retVal, int fd, int readByBuffer, int * bytesRead);
int readCharFromFile(int fd);
long int stringToLong(char * number);
int reverseFile(int fd);

static long int maxRead;

int main(int argc, char** argv)
{
    if(argc!=2)
    {
        err(1, "incorect usage, run ./a.out <fifoname>");
    }
    
    char * fifoname = argv[1];
    int len = strlen(fifoname) +1; //term null char
    if(len>MAX_FILENAME_LEN)
    {
        err(1, "fifoname too long");
    }
    if(mkfifo(fifoname, 0666)==-1)
    {
        if(errno!=EEXIST)
        {
            //some other error then fifos existence occured
            errx(1,"error creating fifo");
        }
        DEBUG_PRINT("fifo already exists\n");
    }

    int fifofd= open(fifoname, O_RDONLY); //flags?
    if(fifofd==-1)
    {
        errx(1,"opening fifo failed");
    }

    int bytesRead;
    int retVal=0;
    while(1) //read from fifo
    {
        char * outputFile = ReadLineFromFile(&retVal, fifofd, 0, NULL);
        if(retVal==-1)
        {   
            return 0;
            //err(1, "input data incomplete");
        }
        DEBUG_PRINT_GREEN("output file name: %s\n", outputFile);

        char * number = ReadLineFromFile(&retVal, fifofd, 0, NULL);
        if(retVal==-1)
        {   
            //return 0;
            err(1, "input data incomplete");
        }
        DEBUG_PRINT_GREEN("string representation of number: %s\n", number);
        long int bytes = stringToLong(number); //would it work with size_t?
        maxRead=bytes;
        DEBUG_PRINT_GREEN("number: %ld\n", bytes);
        free(number);

        //char buf[bytes];
        char * buf;

        DEBUG_PRINT_GREEN("opening output file %s\n", outputFile);
        int outputfd= open(outputFile,O_RDWR|O_CREAT, 0666);
        if(outputfd==-1)
        {
            errx(1,"opening outputfile %s failed", outputFile);
        }

        DEBUG_PRINT_GREEN("reading from fifo\n");

        while(retVal!=1)
        {
            buf = ReadLineFromFile(&retVal,fifofd,1, &bytesRead);
            DEBUG_PRINT_GREEN("%d bytes read from fifo\n", bytesRead);
            int writen = write(outputfd, buf, bytesRead);
            if(writen==-1)
            {
                errx(1,"writing to output file failed");
            }
            DEBUG_PRINT_GREEN("%d bytes writen to output file\n", writen);
            free(buf);
        }
       /* int n = read(fifofd,buf,bytes);
        if(n==-1)
        {
            errx(1,"reading fifo failed");
        }*/
        
        reverseFile(outputfd);
        close(outputfd);

        free(outputFile);
    }
    close(fifofd);
    return 0;
    
}
int reverseFile(int fd)
{
    DEBUG_PRINT_GREEN("entering reverseFile\n");
    char *addr, *p1, *p2;
    char c;
    int size;

    size = lseek(fd, 0, SEEK_END);

    p1 = addr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	p2 = p1 + size - 1;

	while (p1 < p2)
    {
		c = *p1;
		*p1++ = *p2;
		*p2-- = c;
	}

	munmap(addr, size);
	//close(fd);
	return (0);
}

long int stringToLong(char * number)
{
    errno=0;
    long int res = strtol(number,NULL, 10);
    if(errno!=0)
    {
        errx(1, "error while converting string to number");
    }
    return res;
}


int readCharFromFile(int fd)
{
    static char buffer[CHAR_BUFFER_SIZE];
    static int posRead=0;
    static int posBuffed=0;

    if(posRead>=posBuffed)
    {
        //we need to read new buffer from file
        int charsRead = read(fd, buffer, CHAR_BUFFER_SIZE);
        if(charsRead==(-1))
        {
            //error while readning, errno is set
            err(1, "error while reading file");
        }
        if(charsRead==0)
        {
            DEBUG_PRINT("END OF FILE\n");
            return -1;
        }
        else
        {
            DEBUG_PRINT("readChar: reading first char from a new buffer\n");
            posBuffed = charsRead;
            posRead = 0;
            DEBUG_PRINT("readChar: pos read: %d, char to return: %c\n", posRead, buffer[posRead]);
            return buffer[posRead++];
        }
    }
    else
    {
        DEBUG_PRINT("readChar: returning char\n");
        DEBUG_PRINT("readChar: pos read: %d, char to return: %c\n", posRead, buffer[posRead]);
        return buffer[posRead++];
    }
    
}

char *  ReadLineFromFile(int * retVal, int fd, int readByBuffer, int * bytesRead)
{
    
    char *line;
    SAFE_MALLOC( line, LINE_BUFFER_SIZE);
    size_t  buffSize=LINE_BUFFER_SIZE;

    int charCounter = 0;
    if(readByBuffer==1)
    {
        DEBUG_PRINT_GREEN("reading rest using buffer");
        while((size_t)charCounter<buffSize)
        {
            if(charCounter==maxRead)
            {
                *retVal = 1;
                *bytesRead = charCounter; //??
                return line;
            }
            int charRead = readCharFromFile(fd);
            if(charRead == (-1))
            {
                DEBUG_PRINT("Read by buffer: end of FILE\n");
                //line[charCounter]='\0'; //terminating null
                DEBUG_PRINT("READLINE FROM FILE:line: %s\n", line);
                //whole input read
                *retVal = 1;
                *bytesRead = charCounter; //??
                return line;
            }

            DEBUG_PRINT("adding char %c, charCounter: %d\n", charRead, charCounter);
            DEBUG_PRINT("line lenght %d\n", (int)strlen(line));
            line[charCounter] = charRead;
            charCounter++;
        }
        
        //buffer filed, some input reamining 
        *retVal = -1;
        *bytesRead=charCounter;
        return line;
        
    }

    while(1)
    {
        int charRead = readCharFromFile(fd);

        if(charRead == (-1))
        {
            DEBUG_PRINT("Read line from file: end of FILE\n");
            line[charCounter]='\0'; //terminating null
            DEBUG_PRINT("READLINE FROM FILE:line: %s\n", line);
            *retVal = -1;
            return line;
        }
        if(charRead == '\n')
        {
            DEBUG_PRINT("Read line from file: end of LINE\n");
            line[charCounter]='\0'; //terminating null
            DEBUG_PRINT("READLINE FROM FILE: line: %s\n", line);
            *retVal = 0;
            return line;
        }
        if((size_t)charCounter >= buffSize)
        {
            DEBUG_PRINT("Read line from file: realoc buffer\n");
            buffSize*=2;
            SAFE_REALLOC(line, buffSize );
            DEBUG_PRINT("new buffer size %d\n", (int)buffSize);
        }

        DEBUG_PRINT("adding char %c, charCounter: %d\n", charRead, charCounter);
        DEBUG_PRINT("line lenght %d\n", (int)strlen(line));
        line[charCounter] = charRead;
        charCounter++;
    }
}