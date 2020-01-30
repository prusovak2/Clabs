#include "debugPrint.h"
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <sys/types.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int currNumTh=0;


void increaseNum()
{
    // pthread_mutex_lock(&mutex);
    DEBUG_PRINT("increasing num from %d\n", currNumTh);
    currNumTh++;
    // pthread_mutex_unlock(&mutex);
}
void deincreaseNum()
{
    // pthread_mutex_lock(&mutex);
    DEBUG_PRINT("deincreasing num from %d\n", currNumTh);
    currNumTh--;
    // pthread_mutex_unlock(&mutex);
}

void * Thread(void *x) //does it need to take param?
{
    // int x = *a; 
    //increaseNum();
    printf("Thread %d started\n", *(int*)x);
    int time = random() % 10;
    DEBUG_PRINT("Thread %d falling asleep for %d sec\n", *(int*)x, time);
    sleep(time);
    pthread_mutex_lock(&mutex);
    deincreaseNum();
    printf("Thread %d finished\n", *(int*)x);
    
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
   // pthread_detach(pthread_self());
}


int main(int argc, char** argv) 
{
    DEBUG_PRINT("entering main\n");
    if(argc!=2)
    {
        errx(42,"DEVISORS COUNTER: invalid amount of args");
    }
    int numThreads = (int)strtol(argv[1],NULL, 10);
    DEBUG_PRINT("numTh: %d\n", numThreads);
    int indexes[numThreads];
    
    //pthread_t arr = malloc(numThreads * sizeof(pthread_t));

    pthread_attr_t a;
	pthread_attr_init(&a);
	pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);

    
	/* Let's not shoot our leg off. */
	assert(sizeof (int) <= sizeof (void *));

    /* Seed the random device. */
	srandom(time(NULL));
    pthread_mutex_lock(&mutex);

    int thCounter=0;

    for (int i = 0; i < numThreads ; i++)
    {
        indexes[i]=i;
        pthread_t th;
        DEBUG_PRINT("creating thread %d\n", i);
	    pthread_create(&th, &a, Thread, (void*)&thCounter);
        thCounter++;
        increaseNum();
    }


    //return 9;


    while (1) 
    {		
        DEBUG_PRINT("%d >= %d (currNumTh>= numThreads)\n",currNumTh, numThreads);
		while (currNumTh>= numThreads) 
        {
			pthread_cond_wait(&cond, &mutex);
			printf("main thread: woken up Curr num of th: %d.\n", currNumTh);

        }
        pthread_t th;
        currNumTh++;
        thCounter++;
        DEBUG_PRINT("creating another thread %d\n", thCounter);
	    int ret = pthread_create(&th, &a, Thread, (void*)&thCounter);
        pthread_detach(th);
	}
    pthread_mutex_unlock(&mutex);

}

