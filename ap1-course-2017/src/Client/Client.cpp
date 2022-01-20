#include "Client.h"

Client::Client(string ip, int port) {
    this->ip = ip;
    this->port = port;
    this->client_socket = -1;
}

Client::~Client() {
    if(d != NULL){
        delete d;
    }

    LDEBUG << "Closing the socket...";
    close(this->client_socket);
}

void Client::Connect() {
    // Init socket
    this->client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket >= 0) {
        // Memset the connection details
        memset(&this->connection_details, 0, sizeof(this->connection_details));
        this->connection_details.sin_family = AF_INET;
        this->connection_details.sin_addr.s_addr = inet_addr(ip.c_str());
        this->connection_details.sin_port = htons(port);
        // Connect to a server
        if (connect(this->client_socket,
                    (struct sockaddr *) &this->connection_details, sizeof(this->connection_details)) >= 0)
            LDEBUG << ">> Successfully opened socket.";
    } else {
        LERROR << ">> Failure: opening socket.";
    }
}

int Client::Start() {
    Connect();
    if (client_socket < 0)
        return EXIT_FAILURE;
    Communicable server(client_socket);

    bool isRunning = false;
    DriverInput *din = NULL;

    LINFO << ">> Waiting for driver input:";
    din = InputParser::inputDriver();

    //if bad input received, exit.
    if (din == NULL) {
        LERROR << ">> Failure:bad input received";
        return EXIT_FAILURE;
    }

    //input the driver
    d = din->driver;
    if (server.sendObject(din)) {
        LINFO << ">> successfully sent the driver to server.";
    } else {
        LERROR << ">> could not send the driver to server, exit.";
        return EXIT_FAILURE;
    }
    //delete the driver input.
    delete din;

    //receive the vehicle from the server
    Message *m = server.receiveMessage();
    if (m != NULL) {
        LINFO << ">> Received Message from the server...";
        if (m->op == OP_OBJECT) {
            Taxi *t = Serialization::deserialize<Taxi>(m->str);
            if (t != NULL) {
                LINFO << ">> Successfully parsed the message into vehicle.";
                d->setVehicle(t);
                isRunning = true;
            }
        }
        //free the message
        delete m;
        m = NULL;
    }else{
        LERROR << ">> Failed to receive vehicle from the server.";
    }

    //main loop
    while (isRunning) {
        m = server.receiveMessage();
        if (m == NULL) {
            LERROR << ">> could not parse the message...";
            continue;
        }
        LDEBUG << ">> Received Message with op code:" << (int)m->op << ".";
        switch (m->op) {
            case OP_END_CONN:
                LINFO << ">> received END_CONNECTION command, finishing...";
                isRunning = false;
                break;
            case OP_MESSAGE:
                //do not do anything.
                break;
            case OP_OBJECT: {
                //expect the object to be a trip.
                Trip *t = Serialization::deserialize<Trip>(m->str);
                LINFO << ">> Received Trip";

                if (t == NULL) {
                    //error, received an object that is not trip.
                    break;
                }
                if (d->assignTrip(t)) {
                    LDEBUG << ">> assigned trip.";
                    //assigned the trip
                } else {
                    LERROR << ">> could not assign trip.";
                }
                break;
            }
            case OP_COMMAND: {
                int cmd = NOP;
                LDEBUG << ">> Received a command...";
                //case the message to int.
                try {
                    cmd = atoi(m->str.c_str());
                    LDEBUG << "Successfully parsed the Command :" << cmd << ".";
                } catch (exception) {
                    //if its bad command, do nothing.
                    LERROR << ">>ERROR: bad command.";
                    break;
                }
                //its good command.
                //get the command by using
                switch (cmd) {
                    case GO:
                        LINFO << ">> Received GO command, driving...";
                        d->drive();
                        break;
                    default:
                        LERROR << ">> unknown command received.";
                        break;
                }
                break;
            }
            default:
                LERROR << ">> unknown op code";
                break;
        }
        //delete the message to avoid memory leaks.
        delete m;
    }
    LINFO << ">> Finishing...";
    //free the components
    return 0;
}
