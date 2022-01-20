#include "Trip.h"

/**
 * Trip default Ctor.
 */
Trip::Trip() {
    _tripInfo = NULL;
    _current = Point(-1,-1);
}


/**
 * Trip Constructor
 * @param tripInfo
 * @param path
 */
Trip::Trip(TripInfo *tripInfo, const deque<Point> &path) {
    if(tripInfo == NULL){
        throw "Trip::Trip: null tripInfo received.";
    }

    //if received path that does not end on the endPoint of the tripInfo, throw exception.
    if(!(tripInfo->getEndPoint() == path.back())){
        throw "Path does not end on the endPoint of the tripInfo!";
    }
    _tripInfo = tripInfo;
    _path = path;
    _current = _tripInfo->getStartPoint();
}

Trip::Trip(TripInfo *tripInfo, const vector<Point> &path){
    if(tripInfo == NULL){
        throw "Trip::Trip: null tripInfo received.";
    }

    //if received path that does not end on the endPoint of the tripInfo, throw exception.
    if(!(tripInfo->getEndPoint() == path.back())){
        throw "Path does not end on the endPoint of the tripInfo!";
    }
    _tripInfo = tripInfo;
    copy(path.begin(), path.end(), inserter(_path, _path.end()));
    _current = _tripInfo->getStartPoint();
}

/**
 * Trip Dtor
 */
Trip::~Trip() {
    if(_tripInfo != NULL){
        delete _tripInfo;
        _tripInfo = NULL;
    }
}

/**
 * TripInfo getter
 * @return the trip info received
 */
TripInfo *Trip::getTripInfo() const {
    return _tripInfo;
}

/**
 * Moves the specified ammount of steps in the trip.
 * If negative number given or requested to step after trip has ended, it will return
 * the end point of the tripInfo.
 * returns the current location in the trip after moving.
 * @param steps - the ammount of steps to move in the path
 * @return the current location of the trip after moving.
 */
Point Trip::moveSteps(int steps) {
    LDEBUG << "move "<< steps << "steps";
    while(steps > 0 && !_path.empty()){
        --steps;  //decrease the step number, for the stop condition.
        //pop the currentPoint to our local variable currentPoint
        _current = _path.front();
        _path.pop_front();
    }
    return _current;
}

/**
 * returns true if the trip has finished.
 * @return bool signifies whether the trip has finished.
 */
bool Trip::finished() {
    if(_tripInfo == NULL)
        return true;
    return (_current == _tripInfo->getEndPoint());
}
