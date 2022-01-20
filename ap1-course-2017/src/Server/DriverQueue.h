#ifndef AP1_CLIENTQUEUE_H
#define AP1_CLIENTQUEUE_H


#include "../Util/DriverInput.h"

class DriverQueue {
public:
    DriverQueue(int capacity);

    ~DriverQueue();

    // Retrieves the drivers
    vector<DriverInput *> getAllDrivers();

    // Add a driver
    void addDriver(DriverInput *di);

private:
    unsigned int _capacity;
    std::vector<DriverInput*> _drivers;
    pthread_mutex_t qmtx;
    pthread_cond_t wcond;
};


#endif //AP1_CLIENTQUEUE_H
