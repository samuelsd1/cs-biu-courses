#include "DriverFactory.h"
/**
 * DriverFactory Ctor.
 */
DriverFactory::DriverFactory() {
    _id = 0;
    _age = 0;
    _status = SINGLE;
    _experience = 0;
    _t = NULL;
}
/**
 * DriverFactory Dtor.
 */
DriverFactory::~DriverFactory() {}

/**
 * id setter
 */
void DriverFactory::setID(const int &id) {
    if(id < 0)
        throw "DriverFactory::setID: negative id received";
    _id = (unsigned int)id;
}

/**
 * age setter.
 */
void DriverFactory::setAge(const short &age) {
    if (age < 1) {
        throw "DriverFactory::setAge: negative or zero age received";
    }
    _age = (unsigned short)age;
}

/**
 * DriverFactory Dtor.
 */
void DriverFactory::setMaritalStatus(const char &status) {
    switch (status) {
        case 'S':
            _status = SINGLE;
            break;
        case 'M':
            _status = MARRIED;
            break;
        case 'D':
            _status = DIVORCED;
            break;
        case 'W':
            _status = WIDOWED;
            break;
        default:
            throw "DriverFactory::setMaritalStatus: unrecognized marital status char signifier.";
    }
}

/**
 * experience years setter.
 */
void DriverFactory::setExperienceYears(const short &experience) {
    if (experience < 0) {
        throw "DriverFactory::setExperienceYears: negative experience years received";
    }
    _experience = (unsigned short)experience;
}
/**
 * vehicle setter
 */
void DriverFactory::setVehicle(Taxi *t) {
    _t = t;
}
/**
 * creates and returns the driver by the setted properties.
 */
Driver *DriverFactory::createDriver() {
    Driver *d = new Driver(_id, _age, _status, _experience);
    if (_t != NULL)
        d->setVehicle(_t);
    return d;
}

