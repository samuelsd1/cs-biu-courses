#ifndef AP1_EX1_Driver_H
#define AP1_EX1_Driver_H

#include <boost/serialization/access.hpp>
#include "../Vehicles/Taxi.h"
#include "../Trip/Trip.h"
#include "../Trip/TripEndedNotifier.h"

enum MaritalStatus {
    MARRIED,
    DIVORCED,
    SINGLE,
    WIDOWED
};

class Driver : public TripEndedNotifier{
public:
    Driver();
    virtual ~Driver();

    Driver(unsigned int id, unsigned short age, MaritalStatus maritalStatus, unsigned short experienceYears,
           double avgSatisfaction = 0);

    int getId() const;

    unsigned short getAge() const;
    void setAge(const unsigned short &newAge);

    MaritalStatus getMaritalStatus() const;
    void setMaritalStatus(MaritalStatus newStatus);

    unsigned short getExperienceYears() const;
    void setExperienceYears(unsigned short experienceYears);

    double getAvgSatisfaction() const;

    Taxi *getVehicle() const;
    bool setVehicle(Taxi *t);

    bool assignTrip(Trip *trip);
    bool isOccupied();
    Trip* getTrip();
    bool endTrip();

    void drive();

    /**
    * Print operator overloading.
    * @param output - the output stream to write the driver string representation to.
    * @param D - the Driver to print
    * @return the updated stream after writing the Driver string representation to it.
    */
    friend std::ostream &operator<<(std::ostream &output, const Driver &D);

private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & _id;
        ar & _age;
        ar & _maritalStatus;
        ar & _experienceYears;
        ar & _avgSatisfaction;
        //not serializing vehicle and trip here, no need yet.
    }

    unsigned int _id;
    unsigned short _age;
    MaritalStatus _maritalStatus;
    unsigned short _experienceYears;
    double _avgSatisfaction;
    Taxi *_taxi;
    Trip *_trip;

};

#endif 


