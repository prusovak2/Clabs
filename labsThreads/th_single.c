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
    pthread_mutex_lock(&mutex);
    DEBUG_PRINT("increasing num from %d\n", currNumTh);
    currNumTh++;
    pthread_mutex_unlock(&mutex);
}
void deincreaseNum()
{
    pthread_mutex_lock(&mutex);
    DEBUG_PRINT("deincreasing num from %d\n", currNumTh);
    currNumTh--;
    pthread_mutex_unlock(&mutex);
}

void * Thread(void *x) //does it need to take param?
{
    //increaseNum();
    printf("Thread %d started\n", *(int*)x);
    int time = random() % 10;
    DEBUG_PRINT("Thread %d falling asleep for %d sec\n", *(int*)x, time);
    sleep(time);
    //deincreaseNum();
    pthread_mutex_lock(&mutex);
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

    // pthread_attr_t a;
	// pthread_attr_init(&a);
	// pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);

    
	/* Let's not shoot our leg off. */
	assert(sizeof (int) <= sizeof (void *));

    /* Seed the random device. */
	srandom(time(NULL));

    while(1)
    {
        Thread((void*)(&numThreads));
    }

}

