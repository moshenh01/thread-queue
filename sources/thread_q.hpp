#ifndef QUEUE_H
#define QUEUE_H

#include <queue>
#include <mutex>
#include <iostream>
#include <thread>

#include <condition_variable>


class ThreadQueue {
private:
    std::queue<void*> th_queue;
    std::mutex mutex;
    std::condition_variable notEmpty;
    

public:
    ThreadQueue();
    
    void enqueue(void* item);
    void* dequeue();
    
};

int isPrime(void *number);



#endif // QUEUE_H
