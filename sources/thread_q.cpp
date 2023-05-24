#include "thread_q.hpp"
using namespace std;




ThreadQueue::ThreadQueue() {}


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
    
    lock.unlock();
    notEmpty.notify_one();
    return item;
}

int isPrime(void* num) {

    unsigned int number = *(unsigned int*)num;
    // Check if number is 2 (the only even prime number)
    if (number == 2) {
        return 1;
    }

    // Check if number is less than 2 or even
    if (number < 2 || number % 2 == 0) {
        return 0;
    }

    // Check for divisibility by odd numbers up to the square root of the number
    for (unsigned int i = 3; i * i <= number; i += 2) {
        if (number % i == 0) {
            return 0;
        }
    }

    return 1;
}
