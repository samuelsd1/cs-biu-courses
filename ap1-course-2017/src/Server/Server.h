#ifndef AP1_SERVER_H_
#define AP1_SERVER_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>

#include <pthread.h>

#include "../Util/easylogging++.h"
#include "Tasks/ClientTask.h"
#include "../Core/Threading/ThreadPool.h"
#include "TaxiCenter/TaxiCenter.h"
#include "../Util/InputParser.h"


#define MAX_CLIENTS_AT_ONCE 50

using namespace std;

class Server {
public:
    Server(int port);
    void Start();
    virtual ~Server();
protected:
    void initSocket();
    void initWorld();
    void registerClients(int numOfClients);
private:
    TaxiCenter* center;
    ThreadPool* threadPool;

    int port;
    int server_socket;
    struct sockaddr_in server_details;
    vector<ClientTask*> clients;

};

#endif /* AP1_SERVER_H_ */