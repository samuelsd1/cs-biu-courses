#ifndef AP1_TRIP_H
#define AP1_TRIP_H

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/deque.hpp>
#include "../../Util/easylogging++.h"

#include <deque>
#include "TripInfo.h"

/**
 * Trip is a class that holds a tripInfo and a queue represents the path,
 * used to implement the speed based movement logic of the driver.
 */
class Trip{
public:
    /**
     * Trip default Ctor.
     */
    Trip();

    /**
     * Trip Ctor
     * @param tripInfo - pointer to trip info
     * @param path - deque holds the path between tripInfo's src and tripInfo's dest.
     */
    Trip(TripInfo *tripInfo, const deque<Point> &path);

    /**
     * Trip Ctor
     * @param tripInfo - pointer to trip info
     * @param path - vector holds the path between tripInfo's src and tripInfo's dest.
     */
    Trip(TripInfo *tripInfo, const vector<Point> &path);

    /**
     * Trip Dtor
     */
    virtual ~Trip();

    /**
     * TripInfo getter
     * @return the trip info received
     */
    TripInfo* getTripInfo() const;

    /**
     * Moves the specified ammount of steps in the trip.
     * If negative number given or requested to step after trip has ended, it will return
     * the end point of the tripInfo.
     * returns the current location in the trip after moving.
     * @param steps - the ammount of steps to move in the path
     * @return the current location of the trip after moving.
     */
    Point moveSteps(int steps);

    /**
    * returns true if the trip has finished.
    * @return bool signifies whether the trip has finished.
    */
    bool finished();
private:
    Point _current;
    deque<Point> _path;
    TripInfo *_tripInfo;

    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & _current;
        ar & _path;
        ar & _tripInfo;
    }
};


#endif //AP1_TRIP_H
