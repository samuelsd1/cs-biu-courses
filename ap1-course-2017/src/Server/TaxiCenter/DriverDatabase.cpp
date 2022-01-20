#include "DriverDatabase.h"

DriverDatabase::DriverDatabase() {}

DriverDatabase::~DriverDatabase() {
    for (unsigned int i = 0; i < table.size(); i++) {
        if (table[i] != NULL)
            delete table[i];
    }
    table.clear();
}

bool DriverDatabase::insertDriver(const int &driverID) {
    if(driverID < 0)
        return false;
    DriverInfo *d = findByDriver(driverID);
    //if driver already exists...
    if(d != NULL)
        return false;

    d = new DriverInfo();
    d->driverID = driverID;
    d->vehicleID = UNASSIGNED;
    d->tripID = UNASSIGNED;
    table.push_back(d);
    return true;
}

DriverInfo *DriverDatabase::findByDriver(const int &driverID) const {
    if (driverID < 0)
        return NULL;
    DriverInfo *d = NULL;
    for (unsigned int i = 0; i < table.size(); i++) {
        if (table[i]->driverID == driverID) {
            d = table[i];
            break;
        }
    }
    return d;
}

DriverInfo *DriverDatabase::findByVehicle(const int &vehicleID) const {
    if (vehicleID < 0)
        return NULL;
    DriverInfo *d = NULL;
    for (unsigned int i = 0; i < table.size(); i++) {
        if (table[i]->vehicleID == vehicleID) {
            d = table[i];
            break;
        }
    }
    return d;
}

DriverInfo *DriverDatabase::findByTrip(const int &tripID) const {
    if (tripID < 0)
        return NULL;
    DriverInfo *d = NULL;
    for (unsigned int i = 0; i < table.size(); i++) {
        if (table[i]->tripID == tripID) {
            d = table[i];
            break;
        }
    }
    return d;
}
