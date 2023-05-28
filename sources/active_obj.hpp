#ifndef ACTIVEOBJECT_H
#define ACTIVEOBJECT_H
#include <atomic>

#include "thread_q.hpp"
#include "shared_variables.hpp"
//cpp
class ActiveObject {
private:
    
    ThreadQueue* queue = nullptr;
    void (*func)(void*) = nullptr;
    bool active = false;
    
    
    std::thread thread;
    int itertion;
    int num;
    
   

public:
    ActiveObject(void (*func)(void*));
    ~ActiveObject();
    
    void startThread(){
        thread = std::thread([this](){
            runInternal();
        });
    }
    int getIteration() const {
        return itertion;
    }
    bool isActive() const {
        return active;
    }
    void runInternal();
    ThreadQueue* getQueue();
    void stop();
    
   


    
};
ActiveObject* createActiveObject(void (*func)(void*));
void destroyActiveObject(ActiveObject* obj);
static int numAO = 1;


#endif // ACTIVEOBJECT_H
