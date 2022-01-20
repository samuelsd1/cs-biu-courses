#include "TaxiCenter.h"
#include "../Tasks/CalculatePathTask.h"

/**
 * TaxiCenter Ctor.
 */
TaxiCenter::TaxiCenter(Map *map) : _map(map) {
    _clock = 0;
    driverDatabase = new DriverDatabase();
    tp = new ThreadPool(5);
}

/**
 * TaxiCenter Dtor
 */
TaxiCenter::~TaxiCenter() {
    for (unsigned int i = 0; i < unassignedTaxis.size(); i++) {
        delete unassignedTaxis[i];
    }
    unassignedTaxis.clear();
    for (unsigned int i = 0; i < employees.size(); i++) {
        delete employees[i];
    }
    employees.clear();
    for (unsigned int i = 0; i < trips.size(); i++) {
        delete trips[i]->info;
        delete trips[i]->path;
        delete trips[i];
    }

    if (_map != NULL) {
        delete _map;
        _map = NULL;
    }
    if(tp!=NULL){
        delete tp;
        tp = NULL;
    }

    if(driverDatabase != NULL){
        delete driverDatabase;
        driverDatabase = NULL;
    }

    if(tp != NULL){
        delete tp;
        tp = NULL;
    }
}

/**
 * adds the given driver to the center
 * @param d -the driver
 * @return bool signifies if the driver was added to the center.
 */
bool TaxiCenter::addDriver(Driver *d) {
    if (d == NULL)
        return false;
    //try to add the driver to the database.
    if (driverDatabase->insertDriver(d->getId())) {
        employees.push_back(d); //insert the driver to the drivers list.
        return true;
    }
    return false;
}

/**
 * returns the driver with the given id
 * @param id -the drivers id
 * @return the driver if exists, Null if not.
 */
Driver *TaxiCenter::getEmployee(int id) {
    if (id < 0) {
        return NULL;
    }
    //search for the driver by id and return its pointer
    for (unsigned int i = 0; i < employees.size(); i++) {
        if (employees[i]->getId() == id)
            return employees[i];
    }
    return NULL;
}

/**
 * adds the given taxi to the center
 * @param t -the taxi
 * @return bool signifies if the Taxi was added to the center.
 */
bool TaxiCenter::addTaxi(Taxi *t) {
    if (t == NULL) {
        LERROR << "TaxiCenter::addTaxi(): NULL taxi received.";
        return false;
    }
    //make sure the taxi is not already added to the center
    for (unsigned int i = 0; i < unassignedTaxis.size(); i++) {
        //if there is a taxi with the same id, dont add the given taxi.
        if (unassignedTaxis[i]->getID() == t->getID()) {
            LERROR << "TaxiCenter::addTaxi(): Taxi with id " << t->getID() << " already exists, canceling...";
            return false;
        }
    }
    //add the taxi to the unassigned taxis
    unassignedTaxis.push_back(t);
    return true;
}

/**
 * function assigns taxi with the given ID to the driver with the given ID,
 * and returns bool which signifies the success of the operation.
 * @param taxiID - the taxi ID to assign to the driver
 * @param driverID - the driver to assign the taxi to.
 * @return bool signifies success of the assign.
 */
bool TaxiCenter::assignTaxiTo(int taxiID, int driverID) {
    //if illegal index given.
    if (taxiID < 0 || driverID < 0) {
        LERROR << "either driver or taxi ids or both are negative.";
        LERROR << "driver :"<< driverID << "taxi :"<< taxiID;
        return false;
    }

    int taxiIdx = -1; //to save the taxi index.
    //find the taxi in the unassigned taxis.
    for (unsigned int i = 0; i < unassignedTaxis.size(); i++) {
        //if found the taxi
        if (unassignedTaxis[i]->getID() == taxiID) {
            taxiIdx = i;
            break;
        }
    }
    //if didnt found the taxi to assign
    if (taxiIdx < 0) {
        LERROR << "didnt found the taxi";
        return false;
    }

    //now find the driver
    Driver *d = NULL;
    for (unsigned int i = 0; i < employees.size(); i++) {
        if (employees[i]->getId() == driverID) {
            d = employees[i];
            break;
        }
    }
    //if didnt found the driver
    if (d == NULL) {
        LERROR << "didnt found the driver";
        return false;
    }
    //now check that the driver doesnt already have a taxi
    if (d->getVehicle() != NULL)
        return false;   //driver have a taxi, return false

    //update the table:
    DriverInfo *di = driverDatabase->findByDriver(driverID);
    if (di == NULL) {
        return false;
    }
    di->vehicleID = taxiID;

    //now connect the two.
    Taxi *t = unassignedTaxis[taxiIdx]; //save the taxi
    unassignedTaxis.erase(unassignedTaxis.begin() + taxiIdx); //remove the taxi from the unassigned taxis.
    d->setVehicle(t); //assign the taxi to the driver
    return true;
}

/**
 * adds the trip to the center
 * @param t -the trip
 * @return true if the trip was added to the center.
 */
bool TaxiCenter::addTrip(TripInfo *t) {
    if (t == NULL) {
        LERROR << "TaxiCenter::addTrip: NULL TripInfo received.";
        return false;
    }

    LDEBUG << "Checking that the trip is not in the unassigned trips...";
    //check that the trip is not in the unassigned trips
    for (unsigned int i = 0; i < trips.size(); i++) {
        //if found the trip in the unassigned trips, return false.
        if (trips[i]->info->getRideID() == t->getRideID()){
            LDEBUG << "Trip with id " << t->getRideID() << " is already in unassigned trips. canceling...";
            return false;
        }
    }

    LDEBUG << "Checking that the trip is not inside the assigned trips...";
    //check that the trip is not inside the assigned trips.
    //if found the trip in the assigned trips, return false.
    if (driverDatabase->findByTrip(t->getRideID()) != NULL){
        LDEBUG << "Trip with id " << t->getRideID() << " was already assigned. canceling...";
        return false;
    }


    //by now, the trip is new to the system, its not in the assigned trips nor the unassigned trips.
    //add the trip to the unassigned trip infos, and assign the path calculation to a new task
    LINFO << "Adding trip with id " << t->getRideID() << " to the taxi center...";
    UnassignedTrip *trip = new UnassignedTrip();
    trip->info = t;
    //create new pathCalculation object to calculate the path into
    trip->path = new PathCalculation(t->getRideID());
    //save the UnassignedTrip in the trips vector
    trips.push_back(trip);

    //create new path calculation task and assign it to the threadpool
    LDEBUG << "Creating Task to calculate the path for trip with id " << trip->info->getRideID() << ".";
    tp->addTask(new CalculatePathTask(_map, t, trip->path));
    return true;
}

/**
 * assigns the given trip to a driver, returns the index of the driver that
 * the trip was assigned to.
 * if the trip was not assigned to any driver, it returns -1.
 */
int TaxiCenter::assignTrip(int tripID) {
    if (tripID < 0)
        return -1;
    //find the trip in the unassigned trips
    int unassignedTripIndex = -1;
    UnassignedTrip *ut = NULL;
    for (unsigned int i = 0; i < trips.size(); i++) {
        if (trips[i]->info->getRideID() == (unsigned int) tripID) {
            //we found the trip. check the time:
            if (trips[i]->info->getStartTime() != _clock) {
                return -1;
            }
            //the time is good, save the unassigned Trip and its index.
            unassignedTripIndex = i;
            ut = trips[i];
            break;
        }
    }

    //if didnt find the trip
    if (unassignedTripIndex < 0)
        return -1; //return error.

    //check some impossible errors, just to be sure :)
    if (ut == NULL)
        return -1; //return error

    //find the driver to assign the trip to.
    Driver *d = NULL;
    for (unsigned int i = 0; i < employees.size(); i++) {
        //if the current employee is occupied, we cant assign trip to him. move on to next employee.
        if (employees[i]->isOccupied())
            continue;
        //if the current employee has trip and has vehicle, assign the trip to him.
        if (employees[i]->getVehicle() != NULL) {
            //if the vehicles location is where the trip starts
            if (ut->info->getStartPoint() == employees[i]->getVehicle()->getLocation()) {
                d = employees[i];
                break;
            }
        }
    }

    //if no driver found to assign the trip to:
    if (d == NULL) {
        return -1; //return error.
    }

    // everything went fine, assign the trip:
    // start from removing the trip from the trips vector
    trips.erase(trips.begin() + unassignedTripIndex);

    //extract the components from the unassignedTrips to create new trip.
    TripInfo *info = ut->info;
    vector<Point> path = ut->path->getPath();
    //delete the unassigned trip struct and the calculation object, no more use of them.
    delete ut->path;
    delete ut;

    //if no path found
    if (path.empty()) {
        //delete the tripInfo
        delete info;
        return -1; //return error and do not assign the trip.
    }

    //construct new trip object
    Trip *trip = new Trip(info, path);

    // now, update the database
    DriverInfo *di = driverDatabase->findByDriver(d->getId());
    //if no driver found in the database, critical error
    if (di == NULL) {
        //delete the trip
        delete trip;
        return -1; //return error
    }
    //update the tripID in the row.
    di->tripID = info->getRideID();

    //assign the trip to the driver
    d->assignTrip(trip);

    //return the id of the driver that the trip was assigned to.
    return d->getId();
}

/**
 * assigns all the unassigned trips to the drivers
 * @return vector containing the id's of the drivers that were assigned with trips.
 */
vector<int> TaxiCenter::assignAllTrips() {
    LINFO << "assignAllTrips";
    vector<int> assignedDriversIDs;
    //copy the trips vector to iterate on it, because it might change while iterating on it.
    vector<UnassignedTrip *> copy = trips;
    for (unsigned int i = 0; i < copy.size(); i++) {
        if (trips[i]->info->getStartTime() == _clock) {
            //try to assign the trip
            int res = assignTrip(copy[i]->info->getRideID());
            //if a driver was assigned with the trip
            if (res > 0)
                assignedDriversIDs.push_back(res); //save his id
        }
    }
    //return ID's of the drivers that were assigned with trips
    return assignedDriversIDs;
}

/**
 * informs the taxiCenter that a trip was finished.
 * @param tripID - the id of the trip that was finished
 */
void TaxiCenter::tripEnded(const int &tripID) {
    LINFO << "Trip : "<< tripID << "is over.";
    //search the row in the table
    DriverInfo *di = driverDatabase->findByTrip(tripID);
    //if found the right row(matching with tripID)
    if (di != NULL) {
        //remove the trip from the row, set it to be UNASSIGNED trip.
        di->tripID = UNASSIGNED;
        LINFO << "Trip : "<< tripID << "is over.";
    }
}

/**
 * tells all the drivers to drive.
 */
void TaxiCenter::moveAll() {
    for (unsigned int i = 0; i < employees.size(); i++) {
        employees[i]->drive();
    }
}

void TaxiCenter::updateClock() {
    LDEBUG << "TaxiCenter::updateClock(): updating the time...";
    _clock++;
    LINFO << "TaxiCenter: Updated time: " << _clock;
}


