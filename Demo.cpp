#include <iostream>
#include <cstdlib>
#include <ctime>
#include "active_obj.hpp"

using namespace std;
// First Active Object - Generates random numbers
void generateNumbers(void* arg) {
    //arg is arr[2]
    int* task = static_cast<int*>(arg);
    int N = task[0];
    unsigned int seed = (unsigned int)task[1];

    // Seed the random number generator
    std::srand(seed);

    for (int i = 0; i < N; i++) {
        // Generate a random number of 6 digits
        int number = std::rand() % 1000000;
        
        ActiveObject::pipe[1]->getQueue()->enqueue(&number);
        cout << "1) Generated number: " << number << endl;
        // Sleep for 1 millisecond
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
   

}

// Second Active Object - Checks if number is prime
void checkPrime(void* arg) {
    int number = *(int*)arg;
    int (*func)(void* num) = isPrime;
    bool isPrime = func(&number);
    std::cout << "2) Number: " << number << ", Is Prime: " << std::boolalpha << isPrime << std::endl;

    number += 11;
    ActiveObject::pipe[2]->getQueue()->enqueue(&number);
    
}

// Third Active Object - Prints number and checks if it is prime
void printAndCheckPrime(void* arg) {
    int number = *(int*)arg;
    int (*func)(void* num) = isPrime;
    bool isPrime = func(&number);
    std::cout << "3) Number: " << number << ", Is Prime: " << std::boolalpha << isPrime << std::endl;
    number -= 13;
    ActiveObject::pipe[3]->getQueue()->enqueue(&number);
    
    
}

// Fourth Active Object - Prints the received number, adds 2, and prints the new number
void printAndAddTwo(void* arg) {
    
    int number = *(int*)arg;
    std::cout << "4)Number: " << number << std::endl;
    number += 2;
    std::cout << "4)New number: " << number << std::endl;
    
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: pipeline_st <N> [seed]" << std::endl;
        return 1;
    }

    int N = std::stoi(argv[1]);
    int seed = (argc >= 3) ? std::stoi(argv[2]) : std::time(nullptr);

    std::cout << "N: " << N << ", Seed: " << seed << std::endl;

    // Create the pipeline of Active Objects
    

    
    ActiveObject::pipe[0] = createActiveObject(generateNumbers);
    int task_A [2];
    task_A[0] = N;
    task_A[1] = seed;
    ActiveObject::pipe[0]->getQueue()->enqueue(&task_A);

    ActiveObject::pipe[1] = createActiveObject(checkPrime);

    ActiveObject::pipe[2] = createActiveObject(printAndCheckPrime);
    ActiveObject::pipe[3] = createActiveObject(printAndAddTwo);

   

    // Wait for the pipeline to finish
    std::this_thread::sleep_for(std::chrono::seconds(4));
      // Destroy the pipeline
    destroyActiveObject(ActiveObject::pipe[0]);
    destroyActiveObject(ActiveObject::pipe[1]);
    destroyActiveObject(ActiveObject::pipe[2]);
    destroyActiveObject(ActiveObject::pipe[3]);

    return 0;
}
