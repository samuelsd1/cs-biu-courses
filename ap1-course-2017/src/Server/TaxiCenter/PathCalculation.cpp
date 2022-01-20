#include "PathCalculation.h"

PathCalculation::PathCalculation(int tripID) : _tripID(tripID) {
    done = false;
    pthread_mutex_init(&pmtx, 0);

    // wcond is a condition variable that's signaled
    // when the path is calculated.
    pthread_cond_init(&dcond, 0);
}

PathCalculation::~PathCalculation() {
    // Cleanup pthreads
    pthread_mutex_destroy(&pmtx);
    pthread_cond_destroy(&dcond);
}

int PathCalculation::getID() const {
    return _tripID;
}

void PathCalculation::setPath(const std::vector<Point> &path) {
    //lock the path vector and the done variable
    pthread_mutex_lock(&pmtx);
    if(done){
        // Unlock the mutex
        pthread_mutex_unlock(&pmtx);
        return;
    }

    done = true;
    _path = path;
    // signal that the path was calculated.
    pthread_cond_signal(&dcond);
    // Unlock the mutex
    pthread_mutex_unlock(&pmtx);
}

const std::vector<Point> &PathCalculation::getPath(){
    //lock the done bool and the path vector mutex
    pthread_mutex_lock(&pmtx);
    //wait until calculated
    while (!done)
        pthread_cond_wait(&dcond, &pmtx);
    //unlock the mutex
    pthread_mutex_unlock(&pmtx);
    return _path;
}