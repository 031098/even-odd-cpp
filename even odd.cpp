#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

// Shared data
int counter = 0;
const int MAX_NUMBER = 10;

// Synchronization primitives
mutex mtx;
condition_variable cv;
bool is_even_turn = true;

// Function to print even numbers
void print_even() {
    while (counter <= MAX_NUMBER) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return is_even_turn; });

        if (counter <= MAX_NUMBER && counter % 2 == 0) {
            cout << "Even: " << counter << endl;
            counter++;
            is_even_turn = false;
            cv.notify_all();
        }
    }
}

// Function to print odd numbers
void print_odd() {
    while (counter < MAX_NUMBER) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return !is_even_turn; });

        if (counter < MAX_NUMBER && counter % 2 != 0) {
            cout << "Odd: " << counter << endl;
            counter++;
            is_even_turn = true;
            cv.notify_all();
        }
    }
}

int main() {
    // Create threads
    thread even_thread(print_even);
    thread odd_thread(print_odd);

    // Wait for both threads to finish
    even_thread.join();
    odd_thread.join();

    return 0;
}