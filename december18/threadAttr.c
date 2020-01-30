#include <pthread.h>

int main()
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	void * stack;
	pthread_attr_setstacksize(&attr, 16000);
}
