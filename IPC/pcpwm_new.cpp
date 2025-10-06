#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

const int BUFFER_SIZE = 5;
int buffer[BUFFER_SIZE];
int count = 0;
int in = 0;
int out = 0;

mutex mtx;
condition_variable not_full;
condition_variable not_empty;

void producer(int id) {
    for (int i = 0; i < 10; i++) {
        this_thread::sleep_for(chrono::milliseconds(100));

        int item = (id * 100) + i;

        {
            unique_lock<mutex> lock(mtx);

            while (count == BUFFER_SIZE) {
                cout << "Producer " << id << " waiting... buffer is FULL" << endl;
                not_full.wait(lock);
            }

            buffer[in] = item;
            cout << "Producer " << id << " produced: " << item
                 << " at position " << in << " (count=" << count << ")" << endl;
            in = (in + 1) % BUFFER_SIZE;
            count++;

            not_empty.notify_one();

        }
    }
}

void consumer(int id) {
    for (int i = 0; i < 10; i++) {
        this_thread::sleep_for(chrono::milliseconds(150));

        int item;

        {
            unique_lock<mutex> lock(mtx);

            while (count == 0) {
                cout << "Consumer " << id << " waiting... buffer is EMPTY" << endl;
                not_empty.wait(lock);
            }

            item = buffer[out];
            cout << "Consumer " << id << " consumed: " << item
                 << " from position " << out << " (count=" << count << ")" << endl;
            out = (out + 1) % BUFFER_SIZE;
            count--;

            not_full.notify_one();

        }
    }
}

int main() {
    cout << "=== Producer-Consumer WITH Mutex Synchronization ===" << endl;
    cout << "Buffer Size: " << BUFFER_SIZE << endl;
    cout << "Now with proper synchronization - watch the orderly behavior!\n" << endl;

    thread p1(producer, 1);
    thread p2(producer, 2);
    thread c1(consumer, 1);
    thread c2(consumer, 2);

    p1.join();
    p2.join();
    c1.join();
    c2.join();

    cout << "\n=== Final State ===" << endl;
    cout << "Items in buffer: " << count << endl;
    cout << "Next insert position (in): " << in << endl;
    cout << "Next remove position (out): " << out << endl;

    cout << "\n=== Problems SOLVED! ===" << endl;
    cout << " No race conditions - mutex protects critical section" << endl;
    cout << " Correct buffer full/empty detection" << endl;
    cout << " No data corruption or lost items" << endl;
    cout << " Consistent final count (should be 0)" << endl;
    cout << " Proper waiting and signaling between threads" << endl;

    return 0;
}
