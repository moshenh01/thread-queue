#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include "sources/active_obj.hpp"
#include "sources/thread_q.hpp"
#include "sources/shared_variables.hpp"


using namespace std;


std::vector<int*>    allocatedPointers1;
std::vector<int*>    allocatedPointers2;
std::vector<int*>    allocatedPointers3;
std::vector<int*>    allocatedPointers4;

int numOfPrimes = 0;

ActiveObject* pipee[4]{nullptr};

int isPrime(unsigned int number) {

    
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


int usePrime(int number) {
    //cast to unsigned int
    unsigned int num = (unsigned int)number;
    return isPrime(num);
}


// First Active Object - Generates random numbers
void generateNumbers(void* arg) {
    //arg is arr[2]
    int* task = static_cast<int*>(arg);
    int N = task[0];
    unsigned int seed = (unsigned int)task[1];
    
    // Seed the random number generator
    std::srand(seed);
       
    
       for (int i = 0; i < N; i++) {
        int* gNumber = new int;

        // Check if allocation failed
        if(gNumber == nullptr){
            cout<<"gNumber is null"<<endl;
            exit(1);
        }

        *gNumber = std::rand() % 1000000;
        while(*gNumber < 100000){
            *gNumber = std::rand() % 1000000;
        }
        //cout << "1) Generated number: " << *gNumber << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        pipee[1]->getQueue()->enqueue(gNumber);

         allocatedPointers1.push_back(gNumber);

        
    }

  
   

}

// Second Active Object - Checks if number is prime
void checkPrime(void* arg) {
    int* number = new int;
    if (number == nullptr)
    {
        cout<<"number is null"<<endl;
        exit(1);
    }
    
    // "isPrime" need to be thread safe
    *number = *(int*)arg;
    cout<<*number<<endl;
   
    int ans = usePrime(*number);
    if(ans == 1){
        numOfPrimes++;
    }
  
    bool isPrime = (bool)ans;
    std::cout << std::boolalpha <<isPrime<< std::endl;

    *number += 11;

    
    pipee[2]->getQueue()->enqueue(number);
    allocatedPointers2.push_back(number);
    
}

// Third Active Object - Prints number and checks if it is prime
void printAndCheckPrime(void* arg) {
    int* number = new int;
    if (number == nullptr)
    {
        cout<<"number is null"<<endl;
        exit(1);
    }
    *number = *(int*)arg;
    cout<<*number<<endl;

    // "isPrime" need to be thread safe
   
    int ans = usePrime(*number);
    
    bool isPrime = ans;
    std::cout <<std::boolalpha << isPrime << std::endl;
    *number -= 13;
    
    pipee[3]->getQueue()->enqueue(number);
    allocatedPointers3.push_back(number);
    
    
}

// Fourth Active Object - Prints the received number, adds 2, and prints the new number
void printAndAddTwo(void* arg) {
    int* number = new int;
    *number = *(int*)arg;
    std::cout<< *number << std::endl;
    *number += 2;
    std::cout<< *number << std::endl;
    allocatedPointers4.push_back(number);
    
}

int global_N;
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: pipeline_st <N> [seed]" << std::endl;
        return 1;
    }

    int N = std::stoi(argv[1]);
    global_N = N;
    int seed = (argc >= 3) ? std::stoi(argv[2]) : std::time(nullptr);
    //std::mutex mut;
    //std::condition_variable cond;
    //std::cout << "N: " << N << ", Seed: " << seed << std::endl;
    if(seed <= 0) {
        std::cout << "Seed must be a positive integer" << std::endl;
        return 1;
    }
    // Create the pipeline of Active Objects
    

    
    pipee[0] = createActiveObject(generateNumbers);
   
    pipee[1] = createActiveObject(checkPrime);

    pipee[2] = createActiveObject(printAndCheckPrime);
    pipee[3] = createActiveObject(printAndAddTwo);
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    int task_A [2];
    task_A[0] = N;
    task_A[1] = seed;
    pipee[0]->getQueue()->enqueue(&task_A);
   
    // Wait for the pipeline to finish

    
    //  while( (pipee[3]->isActive() == true)){
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1));
    //  }
    

     std::unique_lock<std::mutex> lock(mtx);
     while( (pipee[3]->getIteration() < N )) {
       
       //cout << "waiting" << endl;
       cond.wait(lock);
     }

    
   // cout << "All threads finished" << endl;

    // cout << "Number of primes: " << numOfPrimes << endl;
    // cout << "prime ratio: " << (double)numOfPrimes/N << endl;

    // Wait for the pipeline to finish
    
    // After using the pointers, deallocate the memory
   
      // Destroy the pipeline
    destroyActiveObject(pipee[0]);
    destroyActiveObject(pipee[1]);
    destroyActiveObject(pipee[2]);
    destroyActiveObject(pipee[3]);
     for (unsigned long i = 0; i < allocatedPointers1.size(); i++) {
        delete allocatedPointers1[i];
    }
    for (unsigned long i = 0; i < allocatedPointers2.size(); i++) {
        delete allocatedPointers2[i];
    }
    for (unsigned long i = 0; i < allocatedPointers3.size(); i++) {
        delete allocatedPointers3[i];
    }
    for (unsigned long i = 0; i < allocatedPointers4.size(); i++) {
        delete allocatedPointers4[i];
    }

    return 0;
}
