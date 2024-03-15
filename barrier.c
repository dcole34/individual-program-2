#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common_threads.h"

// barrier synchronization
typedef struct {

    sem_t lock;         // semaphore for mutual exclusion
    sem_t gate1;   // semaphore for synchronization
    sem_t gate2;   // semaphore for synchronization
    int threadnum;      // number of threads
    int totalthreads;   // total number of threads expected to reach the barrier

} barrier;

barrier bar;    // declare a barrier instance

// function to initialize the barrier
void barrier_initialize(barrier *bar, int totalthreads) {

    Sem_init(&bar->lock, 1);           // initialize lock semaphore
    Sem_init(&bar->gate1, 0);     // initialize gate1 semaphore
    Sem_init(&bar->gate2, 1);     // initialize gate2 semaphore
    bar->threadnum = 0;                 // initialize thread count to 0
    bar->totalthreads = totalthreads;   // set total number of threads

}

// function to wait at the barrier
void barrier_wait(barrier *bar) {

    Sem_wait(&bar->lock);       // acquire lock to update barrier state
    bar->threadnum++;           // increment the thread count

    if (bar->threadnum == bar->totalthreads) {

        Sem_post(&bar->gate2);    // open gate2 if all threads have arrived
        Sem_wait(&bar->gate1);    // wait gate1
    }

    Sem_post(&bar->lock);       // release lock
    Sem_wait(&bar->gate2);    // wait gate2
    Sem_post(&bar->gate2);    // open gate2
    Sem_wait(&bar->lock);       // acquire lock
    bar->threadnum--;           // decrement thread count 

    if (bar->threadnum == 0) {

        Sem_post(&bar->gate1);    // open gate1 if last thread
        Sem_wait(&bar->gate2);    // wait at gate2

    }

    Sem_post(&bar->lock);       // release lock
    Sem_wait(&bar->gate1);    // wait gate1
    Sem_post(&bar->gate1);    // open gate1

}
    // structure to hold thread information
typedef struct {

    int index;      // thread identifier

} threadInfo;

// function executed by each thread
void *childfunction(void *arg) {

    threadInfo *t = (threadInfo *) arg;
    printf("Thread %d: Before barrier\n", t->index); // print before barrier
    barrier_wait(&bar); // wait at barrier
    printf("Thread %d: After barrier\n", t->index);  // print after barrier
    return NULL;

}

int main(int argc, char *argv[]) {

    assert(argc == 2);
    int totalthreads = atoi(argv[1]);   // get total number of threads
    assert(totalthreads > 0);           // makes sue total number of threads is positive

    pthread_t threads[totalthreads];    // array to hold thread IDs
    threadInfo t[totalthreads];        // array to hold thread information

    printf("Parent thread: Starting\n");
    barrier_initialize(&bar, totalthreads);  // initialize the barrier
    int i;
    for (i = 0; i < totalthreads; i++) {
        t[i].index = i; // Assign thread IDs
        Pthread_create(&threads[i], NULL, childfunction, &t[i]);  // create threads

    }

    for (i = 0; i < totalthreads; i++)
        Pthread_join(threads[i], NULL); // Wait for threads to complete
    printf("Parent thread: Ending\n");
    return 0;

}

