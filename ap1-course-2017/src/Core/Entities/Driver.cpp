#include "Driver.h"

/**
 * constructor return the defult driver.
 * @return driver
 */
Driver::Driver() {
    _id = 0;
    _age = 0;
    _maritalStatus = SINGLE;
    _experienceYears = 0;
    _avgSatisfaction = 0;
    _taxi = NULL;
    _trip = NULL;
}

/**
 * Drivers destructor.
 */
Driver::~Driver() {
    //release the trip
    endTrip();
    //release the taxi
    if(_taxi != NULL){
        delete _taxi;
    }
}

/**
 * Driver's Constructor.
 *
 * @param id
 * @param age
 * @param maritalStatus
 * @param experienceYears
 * @param avgSatisfaction
 * @return
 */
Driver::Driver(unsigned int id, unsigned short age, MaritalStatus maritalStatus,
               unsigned short experienceYears, double avgSatisfaction) {
    _id = id;
    _age = age;
    _maritalStatus = maritalStatus;
    _experienceYears = experienceYears;
    _avgSatisfaction = avgSatisfaction;
    _taxi = NULL;
    _trip = NULL;
}

/**
 * ID Getter
 * @return
 */
int Driver::getId() const {
    return _id;
}

/**
 * Age getter
 * @return
 */
unsigned short Driver::getAge() const {
    return _age;
}

/**
 * age Setter
 * @param newAge
 */
void Driver::setAge(const unsigned short &newAge) {
    _age = newAge;
}

/**
 * Marital status getter.
 * @return
 */
MaritalStatus Driver::getMaritalStatus() const {
    return _maritalStatus;
}

/**
 * Marital status setter
 * @param newStatus
 */
void Driver::setMaritalStatus(MaritalStatus newStatus) {
    _maritalStatus = newStatus;
}

/**
 * ExperienceYears getter
 * @return
 */
unsigned short Driver::getExperienceYears() const {
    return _experienceYears;
}

/**
 * experience years setter
 * @param experienceYears
 */
void Driver::setExperienceYears(unsigned short experienceYears) {
    _experienceYears = experienceYears;
}

/**
 * Satisfaction getter
 * @return
 */
double Driver::getAvgSatisfaction() const {
    return _avgSatisfaction;
}

/**
 * Vehicle getter
 * @return
 */
Taxi *Driver::getVehicle() const {
    return _taxi;
}

/**
 * Vehicle setter
 * @param v
 * @return
 */
bool Driver::setVehicle(Taxi *t) {
    if (_taxi != NULL)
        return false;
    _taxi = t;
    return true;
}

/**
 * Assigns trip to the driver
 * @param trip
 * @return
 */
bool Driver::assignTrip(Trip *trip) {
    if (_trip != NULL)
        return false;
    _trip = trip;
    return true;
}

/**
 * Trip getter
 */
Trip *Driver::getTrip() {
    return _trip;
}

/**
 * returns true if the driver has a trip
 * @return bool signifies whether the driver is occupied.
 */
bool Driver::isOccupied() {
    return _trip != NULL;
}

/**
 * ends the current trip.
 * @return bool signifies success of the operation.
 */
bool Driver::endTrip() {
    if (_trip == NULL)
        return false;
    if(_trip->getTripInfo() != NULL){
        //notify all listeners that trip has ended
        notifyAll(_trip->getTripInfo()->getRideID());
    }
    delete _trip;
    _trip = NULL;

    return true;
}

/**
 * makes the driver drive if the driver has a vehicle and a trip to drive by.
 */
void Driver::drive() {
    if(_trip==NULL || _taxi == NULL) {
        return;
    }
    //move the vehicle 'speed' steps, means if the vehicles speed is 2,
    //move the vehicle 2 steps on the map
    _taxi->move(_trip->moveSteps(_taxi->getSpeed()));

    //if the trip is finished, end it using the endTrip function.
    if(_trip->finished()){
        endTrip();
    }
}

std::ostream& operator<<(std::ostream &output, const Driver &D) {
    output << "ID: "<< D._id << ", ";
    output << "Age: " << D._age << ", ";
    output << "Marital Status: " << D._maritalStatus << ", ";
    output << "Experience Years: " << D._experienceYears;
    return output;
}
