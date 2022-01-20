#include "TripInfoFactory.h"

/**
 * TripInfoFactory Ctor.
 */
TripInfoFactory::TripInfoFactory() {
    _id = 0;
    start_x = -1;
    start_y = -1;
    end_x = -1;
    end_y = -1;
    _passengers = 0;
    _tariff = -1;
    _startTime = -1;
}
/**
 * TripInfoFactory destructor.
 */
TripInfoFactory::~TripInfoFactory() {}

/**
 * sets the id of the created tripInfo
 */
void TripInfoFactory::setID(int id) {
    if(id < 0)
        throw "TripInfoFactory::setId: negative ID received";
    _id = (unsigned int)id;
}

/**
 * sets the start time of the created tripInfo
 */
void TripInfoFactory::setStartTime(int time){
    if(time < 0)
        throw "TripInfoFactory::setStartTime: negative startTime received";
    _startTime = time;
}

/**
 * sets the start X coord of the created tripInfo
 */
void TripInfoFactory::setStartX(int startX) {
    start_x = startX;
}

/**
 * sets the start y coord of the created tripInfo
 */
void TripInfoFactory::setStartY(int startY) {
    start_y = startY;
}

/**
 * sets the end x coord of the created tripInfo
 */
void TripInfoFactory::setEndX(int endX) {
    end_x = endX;
}

/**
 * sets the end y coord of the created tripInfo
 */
void TripInfoFactory::setEndY(int endY) {
    end_y = endY;
}

/**
 * sets the number of passengers of the created tripInfo
 */
void TripInfoFactory::setNumOfPassengers(int numOfPassengers) {
    if(numOfPassengers < 0)
        throw "TripInfoFactory::setNumOfPassengers: negative number of passengers received.";
    _passengers = (unsigned int)numOfPassengers;
}

/**
 * sets the tariff of the created tripInfo
 */
void TripInfoFactory::setTariff(float tariff) {
    if(tariff < 0)
        throw "TripInfoFactory::setTariff: negative tariff received";
    _tariff = tariff;
}

/**
 * Creates and returns a tripInfo using the setted properties.
 * if some of the properties were bad or were not setted, it returns null ptr.
 */
TripInfo *TripInfoFactory::createTrip() {
    if(start_x < 0 || start_y < 0|| end_x < 0 || end_y < 0)
        return NULL;
    return new TripInfo(_id, 0, Point(start_x, start_y), Point(end_x, end_y), _passengers, _tariff, _startTime);
}
