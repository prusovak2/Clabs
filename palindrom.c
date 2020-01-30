#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <unistd.h>
#include <stdio.h>



int main(int argc, char** argv)
{
	if(argc!=2)
	{
		err(1, "Invalid number of args");
	}

	int fd = open(argv[1],O_RDONLY);
	int size = lseek(fd, 0, SEEK_END);
	char* adr = mmap(NULL, size,PROT_READ,MAP_SHARED, fd, 0);

	if(adr == (void*)-1)
	{
                err(1, "Mapping failed");
        }

	char* p1 = adr;
	char* p2 = adr+size-1;

	while(p1<p2)
	{
		printf("'%c' [%x] '%c' [%x]\n", *p1, *p1, *p2, *p2);
		
		if(*p1 != *p2)
		{
			return 1;
		}
		p1++;
		p2--;		
	}
	return 0;

}
