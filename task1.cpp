#include <iostream>
#include <thread>
#include <unistd.h>
#include <mutex> // Mutex library for thread synchronization

using namespace std;

mutex mtx; // Global mutex object accessible by all threads

void foo(int a) {
    sleep(5);
    
    mtx.lock(); // Acquire lock to protect the shared resource
    cout << a << endl; // Critical section: only one thread prints at a time
    mtx.unlock(); // Release lock for the next thread
}

int main() {
    thread threads[20];
    
    for (int i = 0; i < 20; i++){
        threads[i] = thread(foo, i);
    }
    
    for (int i = 0; i < 20; i++){
        threads[i].join();
    }
    
    return 0;
}