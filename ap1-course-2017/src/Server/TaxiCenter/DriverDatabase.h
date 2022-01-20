#ifndef AP1_DRIVERDATABASE_H
#define AP1_DRIVERDATABASE_H


#include <vector>
#include <stdlib.h>
#include "DriverInfo.h"


class DriverDatabase {
public:
    DriverDatabase();

    virtual ~DriverDatabase();

    bool insertDriver(const int &driverID);

    DriverInfo *findByDriver(const int &driverID) const;
    DriverInfo *findByVehicle(const int &vehicleID) const;
    DriverInfo *findByTrip(const int &tripID) const;

private:
    std::vector<DriverInfo *> table;
};


#endif //AP1_DRIVERDATABASE_H
