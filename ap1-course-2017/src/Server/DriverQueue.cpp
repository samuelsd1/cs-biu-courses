#include "DriverQueue.h"

DriverQueue::DriverQueue(int capacity) {
    if (capacity < 0)
        throw "DriverQueue::DriverQueue: negative capacity received";
    _capacity = (unsigned int) capacity;

    //initialize mutexes
    pthread_mutex_init(&qmtx, 0);

    // wcond is a condition variable that's signaled
    // when new work arrives
    pthread_cond_init(&wcond, 0);
}

DriverQueue::~DriverQueue() {
    // Cleanup pthreads
    pthread_mutex_destroy(&qmtx);
    pthread_cond_destroy(&wcond);
}

vector<DriverInput *> DriverQueue::getAllDrivers() {
    // The return value
    vector<DriverInput*> drivers;

    // Lock the queue mutex
    pthread_mutex_lock(&qmtx);

    while (_drivers.size() < _capacity)
        pthread_cond_wait(&wcond, &qmtx);

    //copy the drivers to the returned list
    drivers = _drivers;

    // Unlock the mutex and return
    pthread_mutex_unlock(&qmtx);

    //return the drivers.
    return drivers;
}

void DriverQueue::addDriver(DriverInput *di) {
    // Lock the queue
    pthread_mutex_lock(&qmtx);
    // Add the task
    _drivers.push_back(di);
    // signal there's new work
    pthread_cond_signal(&wcond);
    // Unlock the mutex
    pthread_mutex_unlock(&qmtx);
}
