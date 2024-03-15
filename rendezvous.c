#include <stdio.h>
#include <unistd.h>
#include "common_threads.h"

sem_t s1;
sem_t s2;

void *childfunction1(void *arg) {

    printf("Child thread 1: Before\n");
    Sem_post(&s1); // signal semaphore s1 to indicate child 1 is at the rendezvous point
    Sem_wait(&s2); // wait for semaphore s2 to ensure synchronization with child 2
    printf("Child thread 1: After\n");
    return NULL;

}

void *childfunction2(void *arg) {

    printf("Child thread 2: Before\n");
    Sem_post(&s2); // signal semaphore s2 to indicate child 2 is at the rendezvous point
    Sem_wait(&s1); // wait for semaphore s1 to ensure synchronization with child 1
    printf("Child thread 2: After\n");
    return NULL;

}

int main() {

    pthread_t childthread1, childthread2;
    printf("Parent thread: Starting\n");
    Sem_init(&s1, 0); // initialize semaphores
    Sem_init(&s2, 0);
    Pthread_create(&childthread1, NULL, childfunction1, NULL); // create child threads
    Pthread_create(&childthread_2, NULL, childfunction2, NULL);
    Pthread_join(childthread1, NULL); // wait for child threads to finish
    Pthread_join(childthread2, NULL);
    printf("Parent thread: Ending\n");
    return 0

}

