#ifndef PROG_H
#define PROG_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



//*************queue*************

#define QUEUE_SIZE 5

typedef struct {
    void** buffer;
    int capacity;
    int front;
    int rear;
    int count;
    pthread_mutex_t mutex;
    pthread_cond_t full;
    pthread_cond_t empty;
} Queue,*pQueue;

//PUNC
void initializeQueue(Queue* queue, int capacity);
void enqueue(Queue* queue, void* item);
void* dequeue(Queue* queue);
void destroyQueue(Queue* queue);
void* producer(void* arg);
void* consumer(void* arg);

//*************queue*************


//*************active_obj*************
typedef struct {
    pthread_t thread;
    Queue* queue;
    void (*func)(void*);
    int active;
} ActiveObject;

ActiveObject* createActiveObject(void (*func)(void*));
void* runActiveObject(ActiveObject* obj);
Queue* getQueue(ActiveObject* obj);
void stop(ActiveObject* obj);
//*************active_obj*************

int isPrime(unsigned int number);


#endif