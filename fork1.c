#include <unistd.h>
#include <sys/types.h>

int main()
{
	printf("%d\n", getpid() );
	fork();
	fork();
	fork();
	fork();
	sleep(50);
}

