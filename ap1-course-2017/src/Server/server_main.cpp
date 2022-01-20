#include "Server.h"

using namespace easyloggingpp;

_INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[]) {
    Loggers::disableAll();
    //if there are not enough
    if (argc != 2) {
        cout << "Usage: Server: [<port>]" << endl;
        return EXIT_FAILURE;
    }
    //initialize connection.
    //check the received port
    int port = atoi(argv[1]);
    if (port < 0 || port > 65536) {
        //if illegal port received, exit
        cout << "Error: Illegal port: " << port << endl;
        return EXIT_FAILURE;
    }

    //start server.
    Server server(port);
    server.Start();
}