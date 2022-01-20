#include "CalculatePathTask.h"

CalculatePathTask::CalculatePathTask(const Map *map, const TripInfo *tripInfo, PathCalculation *p)
        : _map(map), _tripInfo(tripInfo), _p(p) {
    //if null ptrs given
    if (_map == NULL || _tripInfo == NULL || _p == NULL){
        LERROR << "CalculatePathTask(): NULL ptr received, aborting...";
        abort();
    }
}

CalculatePathTask::~CalculatePathTask() {}

void CalculatePathTask::run() {
    LDEBUG << "Starting to calculate the path of the trip with id " << _tripInfo->getRideID();
    //calculate the path and set it
    vector<Point> path = _map->findPath(_tripInfo->getStartPoint(), _tripInfo->getEndPoint());
    //set the path
    _p->setPath(path);
    LDEBUG << "Finished calculating the path of the trip with id " << _tripInfo->getRideID() << ". Deleting the task...";
    // this thread pool implementation doesn't delete
    // the objects so we perform the cleanup here
    delete this;
}
