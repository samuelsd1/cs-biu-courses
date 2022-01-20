#ifndef AP1_EX1_Taxi_H
#define AP1_EX1_Taxi_H

#include "Vehicle.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

class Taxi : public Vehicle{
public:
    Taxi();

	Taxi(const Point & location,
         const int & id, unsigned int kilometers,
         short speed, CarManufacturer manufacturer, Color color);
	virtual ~Taxi();
	float getTariff();
	void setTariff(float newTariff);

    /**
    * Print operator overloading.
    */
    friend std::ostream &operator<<(std::ostream &output, const Taxi &V);

private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Vehicle>(*this);
        ar & _tariff;
    }

	float _tariff;
};

#endif 