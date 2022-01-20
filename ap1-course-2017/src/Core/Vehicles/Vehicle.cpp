#include "Vehicle.h"

/**
 * Default Ctor
 */
Vehicle::Vehicle() {
    _location = Point(-1,-1);
    _id = -1;
    _kilometers = 0;
    _manufacturer = MANUFACTURER_DEFAULT;
    _color = COLOR_DEFAULT;
}

/**
 * Vehicle constructor.
 * @param location
 * @param world
 * @param id
 * @param kilometers
 * @param manufacturer
 * @param color
 * @param speed
 */
Vehicle::Vehicle(const Point &location, int id, unsigned int kilometers,
                 CarManufacturer manufacturer,
                 Color color, short speed) {
    _location = location;
    _id = id;
    _kilometers = kilometers;
    _manufacturer = manufacturer;
    _color = color;
    setSpeed(speed);
}

/**
 * Vehicle destructor
 */
Vehicle::~Vehicle() {}

/**
 * Vehicle ID getter.
 * @return id.
 */
int Vehicle::getID() {
    return _id;
}

/**
 * Vehicle kilometers getter.
 * @return kilometers.
 */
unsigned int Vehicle::getKilometers() {
    return _kilometers;
}

/**
 * Vehicle manufacturer getter.
 * @return manufacturer.
 */
CarManufacturer Vehicle::getManufacturer() const {
    return _manufacturer;
}

/**
 * Vehicle kilometers setter.
 * @param
 */
void Vehicle::setKilometers(unsigned int km) {
    _kilometers = km;
}

/**
 * Vehicle manufacturer setter.
 * @param
 */
void Vehicle::setManufacturer(const CarManufacturer &c) {
    _manufacturer = c;
}

/**
 * Vehicle color getter.
 * @return color.
 */
Color Vehicle::getColor() const {
    return _color;
}

/**
 * Vehicle color setter.
 * @param
 */
void Vehicle::setColor(Color c) {
    _color = c;
}

/**
 * Vehicle speed getter.
 * @return speed.
 */
unsigned short Vehicle::getSpeed() const {
    return _speed;
}

/**
 * Vehicle speed setter.
 * @param
 */
void Vehicle::setSpeed(const short &speed) {
    if (speed < 0) {
        throw "Vehicle::setSpeed: negative speed received";
    }
    _speed = (unsigned short) speed;
}

/**
 * location getter
 * @return the location of the vehicle.
 */
Point Vehicle::getLocation() {
    return _location;
}

/**
 * moves the vehicle to the rececived point
 * @param dest - the destination to move the vehicle to.
 */
void Vehicle::move(const Point &dest) {
    _location = dest;
}