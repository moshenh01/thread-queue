#ifndef ACTIVEOBJECT_H
#define ACTIVEOBJECT_H

#include "thread_q.hpp"
//cpp
class ActiveObject {
private:
    
    ThreadQueue* queue = nullptr;
    void (*func)(void*) = nullptr;
    bool active = false;
    std::mutex mut;
    std::condition_variable cond;
    std::thread thread;
    int itertion;
    
   

public:
    ActiveObject(void (*func)(void*));
    ~ActiveObject();
    
    void startThread(){
        thread = std::thread([this](){
            runInternal();
        });
    }
    int getIteration() {
        return itertion;
    }
    void runInternal();
    ThreadQueue* getQueue();
    void stop();
   


    
};
ActiveObject* createActiveObject(void (*func)(void*));
void destroyActiveObject(ActiveObject* obj);


#endif // ACTIVEOBJECT_H
