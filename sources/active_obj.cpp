#include "active_obj.hpp"
using namespace std;
ActiveObject::ActiveObject(void (*func)(void*)) {
    this->func = func;
    this->active = true;
    this->queue = new ThreadQueue();
    this->itertion = 0;
}

ActiveObject::~ActiveObject() {
    delete queue;
}

void ActiveObject::runInternal() {
    while (void* task = queue->dequeue()) {
        if(*(int*)task == 0) {
            //cout << "break" << endl;
            break;
        }
        
        func(task);
        this->itertion++; 
        //cout<<"iteration: "<<itertion<<endl;

    }
    
}


ThreadQueue* ActiveObject::getQueue() {
    return this->queue;
}

void ActiveObject::stop() {
    active = false;
    //cencel thread
    int end = 0;
    queue->enqueue(&end);
    thread.join();
    delete queue;
    //call destructor
    queue = nullptr;
}



ActiveObject* createActiveObject(void (*func)(void*)) {
    ActiveObject* obj = new ActiveObject(func);
    obj->startThread();
    return obj;
}

void destroyActiveObject(ActiveObject* obj) {
    obj->stop();
    delete obj;
}