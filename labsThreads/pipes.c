#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <err.h>

#define WRITE_END 1
#define READ_END 0

int main(int argc, char const *argv[])
{
    int test =0;

    printf("pipes.c main entered\n");
    pid_t pids[10];
    int pipes[9][2];

for (int i = 0; i < 10; i++)
{
    if(i<9)
    {
        fprintf(stderr,"pipe %d created\n",i);
        pipe(pipes[i]);
    }
    
    pids[i] = fork();
    if(pids[i]==0)
    {
        //test++;
        fprintf(stderr,"child %d, test:%d\n", i,test);
        
        fprintf(stderr,"child %d started\n",i);
        if(i>=2)
        {
            for (int j = 0; j <= i-2; j++)
            {
                fprintf(stderr,"child %d closing fd %d\n", i, j);
                close(pipes[j][WRITE_END]);
                close(pipes[j][READ_END]);
            }
            
        }
        
        if(i<9)
            {
                //not in last
                close(1); //close stdout
                //redirect output to wr. end of pipe i
                int out = dup(pipes[i][WRITE_END]);
                if(out!=1)
                {
                    fprintf(stderr,"SOME AWEFUL MISTAKE\n");
                }
                close(pipes[i][READ_END]);


                //just trying it out
                close(pipes[i][WRITE_END]);
            }
        
        if(i != 0)
            {
                //not in first
                close(0); //close stdin
                int in= dup(pipes[i-1][READ_END]);
                close(pipes[i-1][WRITE_END]);
                if(in!=0)
                {
                    fprintf(stderr,"SOME AWEFUL MISTAKE\n");
                }

                //just trying it out
                close(pipes[i-1][READ_END]);                
            }

            if(i==0)            
            {
                fprintf(stderr,"executing echo\n");
                execlp("/bin/echo", "/bin/echo praise the sun!", NULL);
                fprintf(stderr,"echo failed\n");
            }
            
            
            fprintf(stderr,"executing cat %d\n", i);
            execlp("/bin/cat", "/bin/cat", NULL);
            fprintf(stderr, "Failed to execute cat number: %d\n", i);
            //should not be reached
            exit(0);        
    }

    
    //parent
   /* if(i>0)
    {
        //each child should inherit just fds to pipes it reads from /writes to
        close(pipes[i-1][WRITE_END]);
        close(pipes[i-1][READ_END]); 
    }*/
}

test++;
for (int i = 0; i < 9; i++)
    {
        fprintf(stderr,"parent: closing fd %d\n",i);
        close(pipes[i][WRITE_END]);
        close(pipes[i][READ_END]);
    }

for (int i = 0; i < 10; i++)
    {
        fprintf(stderr, "P: waiting for child %d\n", i);
        int status;
        waitpid(pids[i],&status,NULL);
       /* while(0 == waitpid(pids[i] , &status , WNOHANG))
        {             
            sleep(1);
            printf("P: waitpid %d\n",i);
        }*/
        
    }
     

}


    

 /*   pid=fork();

    if(pid==0)
    {
        dup2(fd1[READ_END], STDIN_FILENO);
        dup2(fd2[WRITE_END], STDOUT_FILENO);
        close(fd1[WRITE_END]);
        close(fd1[READ_END]);
        execlp(scmd, scmd, secarg,(char*) NULL);
        fprintf(stderr, "Failed to execute '%s'\n", scmd);
        exit(1);
    }
    else
    {
        int status;
        close(fd1[READ_END]);
        close(fd1[WRITE_END]);
        waitpid(pid, &status, 0);
    }*/
    
