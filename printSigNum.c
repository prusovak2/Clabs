#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <err.h>
#include <assert.h>

int main()
{
	pid_t pid;
	pid = fork();
	if(pid==-1)
	{
		err(1, "fork failed");
	}
	if(pid==0) //I'm a child
	{
		printf("Child's PID: %d\n", (int)getpid());
		sleep(120);
		return 0;
	}
	
	pid_t pid2;
	int status;

	pid2 = waitpid(pid, &status, WUNTRACED);

	if(WIFSIGNALED(status))
	{
		printf("Child was kill by signal %d\n", (int)WTERMSIG(status));
	}



}
