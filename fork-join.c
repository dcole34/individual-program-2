#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

sem_t semaphore;

void *childfunction(void *arg) {

    printf("Child thread is here\n");
    Sem_post(&semaphore); // signal semaphore to indicate child thread execution
    return NULL;

}

int main() {

    pthread_t childthread;
    printf("Parent thread: Starting\n");
    Sem_init(&semaphore, 0); // initialize semaphore
    Pthread_create(&childthread, NULL, childfunction, NULL);
    Sem_wait(&semaphore); // wait for child thread using semaphore
    printf("Parent thread: Ending\n");
    return 0;

}

