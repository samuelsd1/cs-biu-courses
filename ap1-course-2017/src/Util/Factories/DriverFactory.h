#ifndef AP1_DRIVERFACTORY_H
#define AP1_DRIVERFACTORY_H

#include "../../Core/Entities/Driver.h"

class DriverFactory {

public:
    DriverFactory();
    virtual ~DriverFactory();

    void setID(const int & id);

    void setAge(const short & age);

    void setMaritalStatus(const char & status);

    void setExperienceYears(const short & experience);

    void setVehicle(Taxi *t);

    Driver* createDriver();
private:
    unsigned int _id;
    unsigned short _age, _experience;
    MaritalStatus _status;
    Taxi *_t;
};


#endif //AP1_DRIVERFACTORY_H
