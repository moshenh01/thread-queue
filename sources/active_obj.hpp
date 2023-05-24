#ifndef ACTIVEOBJECT_H
#define ACTIVEOBJECT_H

#include "thread_q.hpp"
//cpp
class ActiveObject {
private:
    
    ThreadQueue* queue;
    void (*func)(void*);
    bool active;

public:
    ActiveObject(void (*func)(void*));
    ~ActiveObject();
    
    pthread_t thread;
    static ActiveObject *pipe[4];
    
    void runInternal();
    
    ThreadQueue* getQueue();
    void stop();

protected:
    
};

ActiveObject* createActiveObject(void (*func)(void*));
void destroyActiveObject(ActiveObject* obj);


#endif // ACTIVEOBJECT_H
