#include <iostream>
#include "../Util/DriverInput.h"
#include "Client.h"

_INITIALIZE_EASYLOGGINGPP

using namespace easyloggingpp;
using namespace std;

int main(int argc, char *argv[]) {
    Loggers::disableAll();
    //if there are not enough arguments, return.
    if (argc != 3) {
        LERROR << "Usage: Client: [<ip>] [<port>]";
        return EXIT_FAILURE;
    }
    string ip_addr = string(argv[1]);
    int port = atoi(argv[2]);
    //if illegal port received
    if (port < 0 || port > UINT16_MAX) {
        return EXIT_FAILURE;
    }

    LINFO << ">> Creating and starting the client:";
    Client client(ip_addr, port);
    int retVal = client.Start();
    if(retVal!=0){
        LERROR << ">> something happened.";
    }
    return retVal;
}