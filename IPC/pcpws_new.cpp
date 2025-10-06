#include <iostream>
#include <thread>
#include <semaphore.h>
#include <mutex>
#include <chrono>

using namespace std;

const int BUFFER_SIZE = 5;
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t empty;
sem_t full;
mutex mtx;

void producer(int id) {
    for (int i = 0; i < 10; i++) {

        this_thread::sleep_for(chrono::milliseconds(100));

        int item = (id * 100) + i;

        sem_wait(&empty);
        cout << "Producer " << id << " acquired empty slot" << endl;

        {
            lock_guard<mutex> lock(mtx);
            buffer[in] = item;
            cout << "Producer " << id << " produced: " << item
                 << " at position " << in << endl;
            in = (in + 1) % BUFFER_SIZE;
        }

        sem_post(&full);
        cout << "Producer " << id << " signaled full slot" << endl;
    }
}

void consumer(int id) {
    for (int i = 0; i < 10; i++) {
        this_thread::sleep_for(chrono::milliseconds(150));

        sem_wait(&full);
        cout << "Consumer " << id << " acquired full slot" << endl;

        int item;
        {
            lock_guard<mutex> lock(mtx);
            item = buffer[out];
            cout << "Consumer " << id << " consumed: " << item
                 << " from position " << out << endl;
            out = (out + 1) % BUFFER_SIZE;
        }

        sem_post(&empty);
        cout << "Consumer " << id << " signaled empty slot" << endl;
    }
}

int main() {
    cout << "=== Producer-Consumer WITH Semaphore Synchronization ===" << endl;
    cout << "Buffer Size: " << BUFFER_SIZE << endl;
    cout << "Using semaphores for counting and mutex for mutual exclusion!\n" << endl;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    cout << "Semaphore 'empty' initialized to: " << BUFFER_SIZE << endl;
    cout << "Semaphore 'full' initialized to: 0\n" << endl;

    thread p1(producer, 1);
    thread p2(producer, 2);
    thread c1(consumer, 1);
    thread c2(consumer, 2);

    p1.join();
    p2.join();
    c1.join();
    c2.join();

    cout << "\n=== Final State ===" << endl;

    int empty_val, full_val;
    sem_getvalue(&empty, &empty_val);
    sem_getvalue(&full, &full_val);

    cout << "Semaphore 'empty' value: " << empty_val << endl;
    cout << "Semaphore 'full' value: " << full_val << endl;
    cout << "Next insert position (in): " << in << endl;
    cout << "Next remove position (out): " << out << endl;

    sem_destroy(&empty);
    sem_destroy(&full);

    cout << "\n=== How Semaphores Work ===" << endl;
    cout << " 'empty' semaphore counts available slots" << endl;
    cout << " 'full' semaphore counts items in buffer" << endl;
    cout << " Producer waits on 'empty', signals 'full'" << endl;
    cout << " Consumer waits on 'full', signals 'empty'" << endl;
    cout << " Mutex ensures mutual exclusion for buffer access" << endl;

    return 0;
}

