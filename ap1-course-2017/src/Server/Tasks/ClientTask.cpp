#include "ClientTask.h"
#include "../../Util/Serialization/Serialization.h"

ClientTask::ClientTask(int clientDescriptor) : comm(clientDescriptor){
    pthread_mutex_init(&queue_locker, 0);
    _driverID = -1;
}

ClientTask::~ClientTask() {
    // Cleanup pthreads
    pthread_mutex_destroy(&queue_locker);
    //add null object so the loop will end.
    messages.addObj(NULL);
}

void ClientTask::run() {
    LDEBUG << "Starting Client task...";
    Message *m;
    LDEBUG << "Waiting for a message...";
    m = comm.receiveMessage();
    if (m != NULL) {
        if (m->op == OP_OBJECT) {
            DriverInput *di = NULL;
            di = Serialization::deserialize<DriverInput>(m->str);
            if (di != NULL) {
                if (di->driver != NULL) {
                    //save the drivers id
                    _driverID = di->driver->getId();


                    LDEBUG << "Received a driver with id " << _driverID << ".";
                    //lock the queue
                    pthread_mutex_lock(&queue_locker);
                    //add the driver to the queue
                    if (queue != NULL){
                        LDEBUG << "adding the driver to the queue.";
                        queue->addDriver(di);
                        LDEBUG << "Finished adding the driver to the queue.";
                    }

                    //unlock the queue
                    pthread_mutex_unlock(&queue_locker);
                }else{
                    LERROR << "NULL driver received.";
                }
            }else{
                LERROR << "Could not deserialize the driver input.";
            }
        }else{
            LERROR << "The message is not an object.";
        }
        delete m;
    }else{
        LERROR << "NULL Message received.";
    }

    //send data back at him
    while(Message* msg = messages.getNext()){
        comm.sendMessage(msg);
        uint8_t op = msg->op;
        delete msg;
        //if end connection signal sent, exit.
        if(op == OP_END_CONN)
            break;
    }
}

int ClientTask::getClientID() const {
    return _driverID;
}

void ClientTask::sendMessage(Message *m) {
    if(m == NULL){
        LERROR << "ClientTask::sendMessage: NULL Message received";
        throw "ClientTask::sendMessage: NULL Message received";
    }
    messages.addObj(m);
}

void ClientTask::setDriverQueue(DriverQueue *drivers) {
    pthread_mutex_lock(&queue_locker);
    queue = drivers;
    pthread_mutex_unlock(&queue_locker);
}



