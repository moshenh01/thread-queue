#ifndef QUEUE_H
#define QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>


class ThreadQueue {
private:
    std::queue<void*> th_queue;
    std::mutex mutex;
    std::condition_variable notEmpty;
    

public:
    ThreadQueue = default;
    
    void enqueue(void* item);
    void* dequeue();
    
};



#endif // QUEUE_H
