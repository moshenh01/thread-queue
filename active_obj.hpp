#ifndef ACTIVEOBJECT_H
#define ACTIVEOBJECT_H

#include "thread_q.hpp"
//cpp
class ActiveObject {
private:
    pthread_t thread;
    ThreadQueue* queue;
    void (*func)(void*);
    bool active;

public:
    ActiveObject(void (*func)(void*));
    ~ActiveObject();

    void run();
    ThreadQueue* getQueue();
    void stop();
    pthread_t getThread(); // Public getter for the thread
};

ActiveObject* createActiveObject(void (*func)(void*));
void destroyActiveObject(ActiveObject* obj);

#endif // ACTIVEOBJECT_H
