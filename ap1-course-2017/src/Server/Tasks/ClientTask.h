#ifndef AP1_CLIENTTASK_H
#define AP1_CLIENTTASK_H


#include "../../Core/Threading/Task.h"
#include "../../Core/Connection/Communicable.h"
#include "../DriverQueue.h"
#include "../MessageQueue.h"

class ClientTask : public Task {
public:
    ClientTask(int clientDescriptor);

    virtual ~ClientTask();
    void run();
    void showTask() {}

    void setDriverQueue(DriverQueue * drivers);

    int getClientID() const;
    void sendMessage(Message* m);

private:
    DriverQueue *queue;
    pthread_mutex_t queue_locker;

    int _driverID;
    Communicable comm;
    MessageQueue messages;
};

#endif //AP1_CLIENTTASK_H
