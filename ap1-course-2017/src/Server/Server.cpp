#include "Server.h"

Server::Server(int port) : port(port) {
    threadPool = new ThreadPool(0);
    server_socket = -1;
}

Server::~Server() {
    close(this->server_socket);

    for (int i = 0, e = (int) clients.size(); i < e; i++) {
        delete clients[i];
    }
    delete threadPool;
    delete center;
}

void Server::Start() {
    initWorld();
    initSocket();

    int mission;
    do {
        LINFO << "Inputting Mission...";
        cin >> mission;
        switch (mission) {
            case 1: {
                LINFO << "Mission 1 received: Input number of clients and register them.";
                int numOfClients;
                cin >> numOfClients;
                LDEBUG << "Number of clients inputted: " << numOfClients << ", registering them...";
                registerClients(numOfClients);
                LINFO << "Finished registering " << numOfClients << " Clients.";
                break;
            }
            case 2: {
                LINFO << "Mission 2 received: input and add a trip to the taxi center.";
                TripInfo *t = InputParser::inputTripInfo();
                if (t != NULL) {
                    center->addTrip(t);
                    LINFO << "Finished with mission 2.";
                } else
                    LERROR << "Bad TripInfo input, canceling the trip...";

                break;
            }
            case 3: {
                LINFO << "Mission 3 received: input and add a taxi to the taxi center.";
                Taxi *t = InputParser::inputTaxi();
                if (t != NULL)
                    if (center->addTaxi(t)) {
                        LINFO << "Successfully added the taxi " << *t << " to the taxi center.";
                    } else {
                        LERROR << "Could not add the taxi " << *t << " to the taxi center.";
                    }
                else
                    LERROR << "Bad Taxi input, canceling the mission...";
                break;
            }
            case 4: {
                LINFO << "Mission 4 received: input driver ID and print his location";
                int driverID;
                cin >> driverID; //input driver id
                Driver *d = center->getEmployee(driverID);
                //if driver exists
                if (d != NULL) {
                    //if driver has a vehicle
                    if (d->getVehicle() != NULL) {
                        Point loc = d->getVehicle()->getLocation();
                        //print its location
                        LINFO << "Driver ID: " << driverID << ", location: " << loc;
                        cout << loc << endl;
                    } else {
                        LERROR << "The requested driver with id " << driverID
                               << " does not have a location(No assigned taxi)";
                    }
                } else {
                    LERROR << "The requested driver with id " << driverID << " was not found.";
                }
                break;
            }
            case 7: {
                LINFO << "Mission 7 received: close the server.";
                break;
            }
            case 9: {
                LINFO << "Mission 9 received: move and assign trips.";
                LDEBUG << "Updating the time...";
                //update time.
                center->updateClock();

                //move the vehicles - in the server and the client
                // notify the client
                //send all drivers go command.
                LDEBUG << "Sending all clients GO command...";
                for (int i = 0, e = (int) clients.size(); i < e; i++) {
                    clients[i]->sendMessage(Communicable::buildMessage(GO));
                }
                LDEBUG << "Finished sending Go command, moving all drivers in the taxi center...";
                //move the vehicles in the servers.
                center->moveAll(); // move the vehicles in the server

                LDEBUG << "Finished moving all drivers, assigning trips...";
                //assign the trips in the server and the client
                vector<int> driverId = center->assignAllTrips(); //assign the trips in the server
                //if no trips were assigned, break;
                if (driverId.size() < 1) {
                    LDEBUG << "No trips were assigned...";
                    break;
                }

                LDEBUG << "Assigned trips in the taxiCenter, notifying the clients...";
                //assign the trips in the client.
                //iterate on the clients
                for (int i = 0, e = (int) clients.size(); i < e; i++) {
                    for (int j = 0, t = (int) driverId.size(); j < t; j++) {
                        //if a trip was assigned to current client
                        if (clients[i]->getClientID() == driverId[j]) {
                            LDEBUG << "Sending to client with id " << clients[i]->getClientID() << " a trip.";
                            //send that client his trip.
                            Message *m = Communicable::buildMessage(center->getEmployee(driverId[j])->getTrip());
                            clients[i]->sendMessage(m);
                        }
                    }
                }
                LINFO << "Finished with mission 9.";
            }
            default:
                LERROR << "Unrecognized mission received: " << mission << ", continuing...";
                break;
        }
    } while (mission != 7);

    //send close signals for all the clients;
    for (int i = 0, e = (int) clients.size(); i < e; i++) {
        Message *m = Communicable::closeSignal();
        clients[i]->sendMessage(m);
    }
}

void Server::initSocket() {
    LINFO << "Initializing Socket...";
    // Socketing TCP
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        LERROR << "Failure: opening socket.";
        throw "Server::Start(): could not open socket";
    } else {
        LDEBUG << ">> Successfully opened socket.";
    }

    // Server details
    memset(&server_details, 0, sizeof(server_details));
    server_details.sin_family = AF_INET;
    server_details.sin_addr.s_addr = INADDR_ANY;
    server_details.sin_port = htons(port);
    bzero(&(server_details.sin_zero), 8);

    // Binding
    int res = ::bind(server_socket, (struct sockaddr *) &server_details, sizeof(struct sockaddr));
    if (res == -1) {
        LERROR << ">> Failure: binding.";
        throw "Failure at Server::Start: could not bind socket";
    } else
        LDEBUG << ">> Successfully binded.";

    // Listening
    listen(server_socket, MAX_CLIENTS_AT_ONCE);
    LDEBUG << ">> Server is listening on port: " << this->port << ".";
    LINFO << "Finished initializing the socket.";
}

void Server::registerClients(int numOfClients) {
    if (numOfClients < 0)
        return;
    LDEBUG << "Adding " << numOfClients << " Clients to the server...";
    DriverQueue *queue = new DriverQueue(numOfClients);
    threadPool->addWorkerThreads(numOfClients);

    int currentConnections = 0;
    while (currentConnections < numOfClients) {
        int descriptor;

        // Accept a client
        descriptor = accept(server_socket, NULL, NULL);
        if (descriptor >= 0) {
            LDEBUG << "Accepted client with descriptor " << descriptor;
            ClientTask *client = new ClientTask(descriptor);
            client->setDriverQueue(queue);
            clients.push_back(client);
            LDEBUG << "Assigning the client task with descriptor " << descriptor << " to the threadPool...";
            threadPool->addTask(client);
        }
        currentConnections++;
    }

    LDEBUG << "Accepted all clients, now waiting for all of them to send the drivers...";
    vector<DriverInput *> drivers = queue->getAllDrivers();
    LDEBUG << "Got all drivers, adding them to the taxi center";
    //remove the driver queue from all the client tasks to avoid dangling pointer
    for (int i = 0, e = (int) clients.size(); i < e; i++) {
        clients[i]->setDriverQueue(NULL);
    }
    //delete the queue
    delete queue;

    //add the drivers to the center
    for (int i = 0, e = (int) drivers.size(); i < e; i++) {
        DriverInput *di = drivers[i];
        if (di != NULL) {
            if (di->driver != NULL) {
                //save the driver in the taxi center.
                center->addDriver(di->driver);
                di->driver->addTripEndedListener(center);

                //if successfully assigned the vehicle to the driver.
                if (center->assignTaxiTo(di->vehicleID, di->driver->getId())) {
                    //send the vehicle to the client
                    Message *m = Communicable::buildMessage(di->driver->getVehicle());
                    // find the client with the matching driver id and send him the vehicle.
                    for (unsigned int id = 0, t = (unsigned int) clients.size(); id < t; id++) {
                        //if found a match
                        if (clients[id]->getClientID() == di->driver->getId()) {
                            LDEBUG << "Sending Vehicle with id " << di->vehicleID << " to client with driver id "
                                   << clients[id]->getClientID();
                            clients[id]->sendMessage(m);
                            m = NULL;
                            break;
                        }
                    }// clients iteration

                    //if did not found the client somehow, the message was not sent.
                    //delete the message and throw exception.(not supposed to happen).
                    if (m != NULL) {
                        delete di;
                        delete m;
                        std::string err;
                        err += "Fatal Error: Client with id ";
                        err += boost::lexical_cast<std::string>(di->driver->getId());
                        err += " was not found!";
                        LERROR << err;
                        throw err;
                    }
                }//if taxi was assigned
                else {
                    //taxi was not assigned
                    LERROR << "Taxi with id " << di->vehicleID << " was not assigned to the driver with id "
                           << di->driver->getId();
                }
            }//if driver was not null
            else {
                LERROR << "NULL Driver received from Client...";
            }
            delete di;
        }//if driverinput was not null
        else {
            LERROR << "NULL DriverInput received from Client...";
        }
    }//iteration on the driver inputs
}

void Server::initWorld() {
    LINFO << "Initializing World:";
    int width, height;
    char dummy;
    LINFO << "Initializing the map...";
    //input the world dimensions.
    LDEBUG << "Inputting map dimensions";
    cin >> width >> height; // input in format 'width height'
    LINFO << "Inputted Map dimensions: width: " << width << ", height: " << height;
    LDEBUG << "Creating a map with the inputted dimensions.";
    Map *map = new Map(width, height); //create a map
    LINFO << "Finished initializing the map.";

    LINFO << "Initializing Obstacles:";
    //input obstacles
    LDEBUG << "Inputting number of obstacles:";
    cin >> width; //input number of obstacles
    for (int i = 0; i < width; i++) {
        int x, y;
        cin >> x >> dummy >> y; //get obstacle in the format 'x,y'
        LDEBUG << "Inputted obstacle at (" << x << "," << y << "). adding it to the map.";
        map->setObstacleAt(x, y); //insert the obstacle to the map
    }
    LINFO << "Finished initializing the obstacles.";

    //create the taxi center with the inputted map.
    LINFO << "Initializing the Taxi Center...";
    center = new TaxiCenter(map);
    LINFO << "Finished initializing the Taxi Center.";
}

