#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//not finished at all

int openRead(void * file)
{
	open((char*)file,>)
}

int main(int argc char** argv)
{
	int pd[2];

	if (pipe(pd) == -1)
		err(1, "pipe");
	
	pthread_t t;
	(void) pthread_create(&t, NULL, openRead, argv[1]);

}
