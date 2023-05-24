#include "active_obj.hpp"
using namespace std;
ActiveObject::ActiveObject(void (*func)(void*)) {
    this->func = func;
    this->active = true;
    this->queue = new ThreadQueue();
}

ActiveObject::~ActiveObject() {
    delete queue;
}
static void* run(void* arg) {
    ActiveObject* obj = static_cast<ActiveObject*>(arg);
    obj->runInternal();
    return nullptr;
}
void ActiveObject::runInternal() {
    while (void* task = queue->dequeue()) {
        func(task);
      
    }
    
}


ThreadQueue* ActiveObject::getQueue() {
    return this->queue;
}

void ActiveObject::stop() {
    active = false;
    pthread_cancel(thread);
    pthread_join(thread, nullptr);
    //call destructor
    delete queue;
    queue = nullptr;
}
ActiveObject* ActiveObject::pipe[4] = { nullptr, nullptr, nullptr, nullptr };


ActiveObject* createActiveObject(void (*func)(void*)) {
    ActiveObject* obj = new ActiveObject(func);
    pthread_create(&obj->thread, nullptr, &run, obj);
    return obj;
}

void destroyActiveObject(ActiveObject* obj) {
    obj->stop();
    delete obj;
}
