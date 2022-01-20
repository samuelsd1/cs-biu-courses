#ifndef AP1_PATHCALCULATION_H
#define AP1_PATHCALCULATION_H

#include <vector>
#include "../../Core/Point.h"
#include "../../Core/Map.h"

class PathCalculation {
public:
    PathCalculation(int tripID);
    ~PathCalculation();

    int getID() const;
    void setPath(const std::vector<Point> &path);
    const std::vector<Point> &getPath();

private:
    const int _tripID;
    bool done;
    std::vector<Point> _path;

    //mutex for both the path vector and the done, because they are tightly coupled.
    pthread_mutex_t pmtx;

    //conditional variable for the end of the calculation
    pthread_cond_t dcond;
};


#endif //AP1_PATHCALCULATION_H
