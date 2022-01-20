
#ifndef CLIENT_CLIENT_H_
#define CLIENT_CLIENT_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sstream>
#include <stdexcept>
#include "../Core/Entities/Driver.h"
#include "../Util/DriverInput.h"
#include "../Util/InputParser.h"
#include "../Core/Connection/Communicable.h"
#include "../Util/Serialization/Serialization.h"
#include "../Util/easylogging++.h"

using namespace std;

class Client {
public:
    Client(string ip, int port);
    virtual ~Client();

    int Start();

protected:
    void Connect();
private:
    Driver *d;
    string ip;
    int port;
    int client_socket;
    struct sockaddr_in connection_details;
};

#endif /* CLIENT_CLIENT_H_ */