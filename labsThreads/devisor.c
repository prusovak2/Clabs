#include "debugPrint.h"
#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS  8 

int CountDevsOfNUm(int num);
//void CountDevisors(int highets, int* bestCountSoFar, int* bestNumSoFar );
void * Thread(void *x);
void updateBest(int divCount,int num);
int getNextNum();

int bestNumSoFar =1;
int bestCountSoFar=0;
int currNum=0;
int highest;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char** argv) 
{
    if(argc!=2)
    {
        errx(42,"DEVISORS COUNTER: invalid amount of args");
    }
    int high = (int)strtol(argv[1],NULL, 10);
    DEBUG_PRINT("highest: %d", high);
    highest=high;
    pthread_t arr[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS ; i++)
    {
        DEBUG_PRINT("creating thread %d\n", i);
	    pthread_create(&arr[i], NULL, Thread, (void *) 0);
    }
     for (int i = 0; i < NUM_THREADS ; i++)
    {
        DEBUG_PRINT("joining thread %d\n", i);
	    pthread_join(arr[i], NULL);
    }

    
    return 0;
}

void * Thread(void *x) //does it need to take param?
{
    int num = getNextNum();
    while (num<=highest)
    {
        int divCount = CountDevsOfNUm(num);
        updateBest(divCount, num);
        num = getNextNum();
    }
}

void updateBest(int divCount,int num)
{
    pthread_mutex_lock(&mutex);
    if(divCount>bestCountSoFar)
    {
        bestCountSoFar=divCount;
        bestNumSoFar=num;
        printf("best number is %d so far with %d devisors\n", bestNumSoFar, bestCountSoFar);
    }
    pthread_mutex_unlock(&mutex);
}

int getNextNum()
{
    pthread_mutex_lock(&mutex);
    currNum++;
    pthread_mutex_unlock(&mutex);
    return currNum;
}

/*void CountDevisors(int highets, int* bestCountSoFar, int* bestNumSoFar )
{
    *bestCountSoFar =0;
    *bestNumSoFar =1;
    for (int i = 1; i <= highets; i++)
    {
        CountDevsOfNUm(i, bestCountSoFar, bestNumSoFar);
    }
    printf("\n");
    printf("RESULT: best number is %d with %d devisors\n", *bestNumSoFar, *bestCountSoFar);
    
}*/


int CountDevsOfNUm(int num)
{
    DEBUG_PRINT("counting divs of %d\n", num);
    int devCount=0;
    for (int i =1; i<= num; i++)
    {
        int remains = num %i;
        if(remains==0)
        {
            //i is devisor of num
            devCount++;
        }
    }
    return devCount;
    
}