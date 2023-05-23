#include "thread_q.hpp"

void ThreadQueue::enqueue(void* item) {
    std::unique_lock<std::mutex> lock(mutex);
    th_queue.push(item);
    lock.unlock();
    notEmpty.notify_one();
}

void* ThreadQueue::dequeue() {
    std::unique_lock<std::mutex> lock(mutex);
    notEmpty.wait(lock, [this]() { return !th_queue.empty(); });

    void* item = th_queue.front();
    th_queue.pop();

    return item;
}
