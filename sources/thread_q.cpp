#include "thread_q.hpp"

#include "thread_q.hpp"
using namespace std;




ThreadQueue::ThreadQueue():th_queue(),mutex(),notEmpty() {}


void ThreadQueue::enqueue(void* item) {
    std::lock_guard<std::mutex> lock(mutex);
    //cout << "enq "<<*(int*)item << endl;
    th_queue.push(item);
    notEmpty.notify_one();
}


void* ThreadQueue::dequeue() {
    std::unique_lock<std::mutex> lock(mutex);
    while(th_queue.empty()){
       notEmpty.wait(lock);
    }
    void* item = th_queue.front();
    //cout << "deq "<<*(int*)item  << endl;
    th_queue.pop();
    return item;
}