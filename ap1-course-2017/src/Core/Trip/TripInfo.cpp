#include "TripInfo.h"
/**
 * TripInfo default  constructor.
 */
TripInfo::TripInfo() {
    _rideID = 0;
    _totalMeters = 0;
    _start = Point(-1, -1);
    _end = Point(-1, -1);
    _passengers = 0;
    _tariff = 0;
}
/**
 * TripInfo constructor.
 */
TripInfo::TripInfo(unsigned int rideID, unsigned int totalMeters, const Point &startPoint, const Point &endPoint,
                   unsigned int numOfPassengers, float tariff, int startTime) {
    _rideID = rideID;
    _totalMeters = totalMeters;
    _start = startPoint;
    _end = endPoint;
    _passengers = numOfPassengers;
    _tariff = tariff;
    _startTime=startTime;
}
/**
 * RideID`s getter.
 * @return RideID.
 */
unsigned int TripInfo::getRideID() const {
    return _rideID;
}

/**
 * start time`s getter.
 * @return startTime.
 */
int TripInfo::getStartTime() const {
    return _startTime;
}

/**
 * TotalMeters`s getter.
 * @return TotalMeters.
 */
unsigned int TripInfo::getTotalMeters() const {
    return _totalMeters;
}

/**
 * StartPoint getter.
 * @return StartPoint.
 */
Point TripInfo::getStartPoint() const {
    return _start;
}
/**
 * EndPoint getter.
 * @return EndPoint.
 */
Point TripInfo::getEndPoint() const {
    return _end;
}
/**
 * NumOfPassengers getter.
 * @return NumOfPassengers.
 */
unsigned int TripInfo::getNumOfPassengers() const {
    return _passengers;
}
/**
 * Tarifg getter.
 * @return trip`s Tariff.
 */
float TripInfo::getTariff() const {
    return _tariff;
}
