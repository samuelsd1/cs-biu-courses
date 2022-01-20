#include "Taxi.h"

Taxi::Taxi(){
    _tariff = 0;
}

/**
 * Taxi Constructor
 * @return
 */
Taxi::Taxi(const Point & location, const int & id, unsigned int kilometers,
           short speed, CarManufacturer manufacturer, Color color)
        : Vehicle(location, id,kilometers, manufacturer, color, speed){

    _tariff = 1;
}
/**
 * Taxi Dtor
 */
Taxi::~Taxi() {}
/**
 * Tariff getter
 * @return
 */
float Taxi::getTariff(){
	return _tariff;
}
/**
 * Tariff setter
 * @param newTariff
 */
void Taxi::setTariff(float newTariff){
	if(newTariff > 0){
		_tariff = newTariff;
	}
}

/**
 * Print operator overloading.
 * @param output - the output stream to write the point string representation to.
 * @param P - the point
 * @return the updated stream after writing the point string representation to it.
 */
std::ostream& operator<<(std::ostream &output, const Taxi &T) {
    output << "Taxi: {";
    output << "ID: " << T._id << ", location: " << T._location;
    output << ", Kilometers: " << T._kilometers << ", Manufacturer: ";
    switch (T._manufacturer){
        case HONDA:
            output << "Honda";
            break;
        case TESLA:
            output << "Tesla";
            break;
        case FIAT:
            output << "Fiat";
            break;
        case SUBARO:
            output << "Subaro";
            break;
        default:
            output << "UNDEFINED";
            break;
    }
    output << ", Color: ";
    switch(T._color){
        case WHITE:
            output << "White";
            break;
        case PINK:
            output << "Pink";
            break;
        case RED:
            output << "Red";
            break;
        case BLUE:
            output << "Blue";
            break;
        case GREEN:
            output << "Green";
        default:
            output << "UNDEFINED";
    }
    output << ", Tariff: " << T._tariff << "}";
    return output;
}