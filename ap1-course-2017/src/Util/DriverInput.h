
#ifndef AP1_DRIVERINPUT_H
#define AP1_DRIVERINPUT_H

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "../Core/Entities/Driver.h"

struct DriverInput{
    Driver* driver;
    int vehicleID;

    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & driver;
        ar & vehicleID;
    }
};

#endif //AP1_DRIVERINPUT_H
