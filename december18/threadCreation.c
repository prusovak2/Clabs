#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void * doSomething(void * tCount)
{
	int i;

	for (i = 0; i < 5; ++i) {
		(void) printf("thread loop #%d of thread %d\n", i, (int)tCount);
		(void) sleep(1);
	}
	return (NULL);
}

//this code is total mess writen to play around with threads, do not use it lateron!

int main()
{
	pthread_t t1, t2, t3;
	int err;
	void *p1, *p2, *p3;
	
	int tCount =1;
	
	if((err=pthread_create(&t1,NULL, doSomething, tCount))!=0)		
	{
		printf("err while creating thread %d", 1);
	}

	tCount=2;
	
	printf("after first thread creation\n");

	 if((err=pthread_create(&t2,NULL, doSomething,  tCount))!=0)
        {
                printf("err while creating thread %d", 2);
        }
		
	printf("after second thread creation\n");

	tCount=3;

	 if((err=pthread_create(&t3,NULL, doSomething,  tCount))!=0)
        {
                printf("err while creating thread %d", 3);
        }

	printf("after third thread creation\n");

	pthread_join(t1, &p1);
	pthread_join(t2, &p2);
	pthread_join(t3, &p3);

	printf("after join\n");

}


