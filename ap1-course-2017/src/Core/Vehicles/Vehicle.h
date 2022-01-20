#ifndef AP1_EX1_Vehicle_H
#define AP1_EX1_Vehicle_H

#include <boost/serialization/access.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/export.hpp>
#include "../Point.h"

enum CarManufacturer{
    MANUFACTURER_DEFAULT,
    HONDA,
    SUBARO,
    TESLA,
    FIAT
};
enum Color{
    COLOR_DEFAULT,
    RED,
    BLUE,
    GREEN,
    PINK,
    WHITE
};
enum Direction{

};
class Vehicle {
public:

    Vehicle();

    Vehicle(const Point & location,
            int id, unsigned int kilometers,
            CarManufacturer manufacturer, Color color, short speed = 1);
    virtual ~Vehicle() = 0;
	int getID();
	unsigned int getKilometers();
	CarManufacturer getManufacturer() const;
	void setManufacturer(const CarManufacturer & c);
	Color getColor() const;
	void setColor(Color c);
	unsigned short getSpeed() const;

    Point getLocation();
    void move(const Point & destination);

protected:
	void setKilometers(unsigned int km);
	void setSpeed(const short & speed);

    Point _location;
    int _id;
    unsigned int _kilometers;
    CarManufacturer _manufacturer;
    Color _color;
    unsigned short _speed;

private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & _location;
        ar & _id;
        ar & _kilometers;
        ar & _manufacturer;
        ar & _color;
        ar & _speed;
    }

};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Vehicle)

#endif 