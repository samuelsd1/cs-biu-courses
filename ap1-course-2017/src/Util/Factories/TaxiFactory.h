#ifndef AP1_VEHICLEFACTORY_H
#define AP1_VEHICLEFACTORY_H


#include "../../Core/Vehicles/Taxi.h"

class TaxiFactory {
public:
    TaxiFactory();

    virtual ~TaxiFactory();

    void setID(int id);

    void setType(short taxiType);

    void setManufacturer(char manufacturer);

    void setColor(char color);

    Taxi *createTaxi();

private:
    unsigned int _id;
    unsigned short _type;
    CarManufacturer _manufacturer;
    Color _color;
};


#endif //AP1_VEHICLEFACTORY_H
