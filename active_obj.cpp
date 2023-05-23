#include "active_obj.hpp"

ActiveObject::ActiveObject(void (*func)(void*)) {
    this->func = func;
    this->active = true;
    this->queue = new ThreadQueue();
}

ActiveObject::~ActiveObject() {
    stop();
    delete queue;
}

void ActiveObject::run() {
    while (void* task = queue->dequeue()) {
        func(task);
    }
}

ThreadQueue* ActiveObject::getQueue() {
    return queue;
}

void ActiveObject::stop() {
    active = false;
}
pthread_t ActiveObject::getThread() {
    return thread;
}

ActiveObject* createActiveObject(void (*func)(void*)) {
    ActiveObject* obj = new ActiveObject(func);
    pthread_create(&obj->getThread(), nullptr, (void* (*)(void*))&ActiveObject::run, obj);
    return obj;
}

void destroyActiveObject(ActiveObject* obj) {
    obj->stop();
    pthread_join(obj->getThread(), nullptr);
    delete obj;
}
