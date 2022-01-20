#ifndef AP1_SYNCHRONIZEDQUEUE_H
#define AP1_SYNCHRONIZEDQUEUE_H

#include <pthread.h>
#include <queue>
#include "../Util/easylogging++.h"


template <typename T>
class SynchronizedQueue{
public:
    SynchronizedQueue(){
        pthread_mutex_init(&qmtx, 0);

        // wcond is a condition variable that's signaled
        // when new object arrives
        pthread_cond_init(&wcond, 0);
    }
    virtual ~SynchronizedQueue(){
        // Cleanup pthreads
        pthread_mutex_destroy(&qmtx);
        pthread_cond_destroy(&wcond);
    }

    T* getNext(){
        // The return value
        T *next = NULL;

        // Lock the queue mutex
        pthread_mutex_lock(&qmtx);

        while (objects.empty()) {
            pthread_cond_wait(&wcond, &qmtx);
        }
        next = objects.front();
        objects.pop();
        // Unlock the mutex and return
        pthread_mutex_unlock(&qmtx);
        return next;
    }

    void addObj(T* obj){
        // Lock the queue
        pthread_mutex_lock(&qmtx);
        // Add the object
        objects.push(obj);
        // signal there's new work
        pthread_cond_signal(&wcond);
        // Unlock the mutex
        pthread_mutex_unlock(&qmtx);
    }

private:
    std::queue<T*> objects;
    pthread_mutex_t qmtx;
    pthread_cond_t wcond;
};

#endif //AP1_SYNCHRONIZEDQUEUE_H
