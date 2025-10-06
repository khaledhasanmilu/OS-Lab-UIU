#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

using namespace std;

const int BUFFER_SIZE = 5;
int buffer[BUFFER_SIZE];
int count = 0;
int in = 0;
int out = 0;

void producer(int id) {
    for (int i = 0; i <10; i++) {
        this_thread::sleep_for(chrono::milliseconds(100));

        if (count < BUFFER_SIZE) {
            int item = (id * 100) + i;
            buffer[in] = item;
            cout << "Producer " << id << " produced: " << item
                 << " at position " << in << " (count=" << count << ")" << endl;
            in = (in + 1) % BUFFER_SIZE;
            count++;
        } else {
            cout << "Producer " << id << " found buffer FULL! Item lost." << endl;
        }
    }
}

void consumer(int id) {
    for (int i = 0; i < 10; i++) {
        this_thread::sleep_for(chrono::milliseconds(150));

        if (count > 0) {
            int item = buffer[out];
            cout << "Consumer " << id << " consumed: " << item
                 << " from position " << out << " (count=" << count << ")" << endl;
            out = (out + 1) % BUFFER_SIZE;
            count--;
        } else {
            cout << "Consumer " << id << " found buffer EMPTY! Nothing to consume." << endl;
        }
    }
}

int main() {
    cout << "=== Producer-Consumer WITHOUT Synchronization ===" << endl;
    cout << "Buffer Size: " << BUFFER_SIZE << endl;
    cout << "Watch for race conditions and inconsistent behavior!\n" << endl;

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

    cout << "\n=== Problems You Should Observe ===" << endl;
    cout << "1. Race conditions on 'count' variable" << endl;
    cout << "2. Incorrect buffer full/empty detection" << endl;
    cout << "3. Data corruption or lost items" << endl;
    cout << "4. Inconsistent final count" << endl;
    cout << "5. Multiple threads accessing same positions" << endl;

    return 0;
}


