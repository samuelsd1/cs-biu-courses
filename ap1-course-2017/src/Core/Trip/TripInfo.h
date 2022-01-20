#ifndef AP1_EX1_Trip_H
#define AP1_EX1_Trip_H

#include <vector>
#include "../Point.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace std;

class TripInfo {
public:
    TripInfo();

    TripInfo(unsigned int rideID, unsigned int totalMeters, const Point &startPoint, const Point &endPoint,
             unsigned int numOfPassengers, float tariff, int startTime);

    int getStartTime() const;
    unsigned int getRideID() const;

    unsigned int getTotalMeters() const;

    Point getStartPoint() const;
    Point getEndPoint() const;

    unsigned int getNumOfPassengers() const;
    float getTariff() const;

private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & _rideID;
        ar & _totalMeters;
        ar & _start;
        ar & _end;
        ar & _passengers;
        ar & _tariff;
        ar & _startTime;
    }

    unsigned int _rideID;
    unsigned int _totalMeters;
    Point _start;
    Point _end;
    unsigned int _passengers;
    float _tariff;
    int _startTime;

};

#endif 
