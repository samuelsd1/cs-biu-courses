#include "TaxiFactory.h"

/**
 * TaxiFactory Ctor
 */
TaxiFactory::TaxiFactory() {
    _id = 0;
    _type = 0;
}

/**
 * TaxiFactory Dtor
 */
TaxiFactory::~TaxiFactory() { }

/**
 * id setter.
 */
void TaxiFactory::setID(int id) {
    if(id < 0)
        throw "TaxiFactory::setID: negative ID received";
    _id = (unsigned int)id;
}

/**
 * taxi type setter
 * @param taxiType -    1:Normal Taxi
 *                      2:Luxury cab.
 */
void TaxiFactory::setType(short taxiType) {
    if(taxiType != 1 && taxiType != 2)
        throw "TaxiFactory::setType: unknown type received";
    _type = (unsigned short)taxiType;
}

/**
 * manufacturer setter.
 * @param manufacturer -'H': Honda
 *                      'S': Subaro
 *                      'T': Tesla
 *                      'F': Fiat
 *                      other char - throws exception.
 */
void TaxiFactory::setManufacturer(char manufacturer) {
    switch(manufacturer){
        case 'H':
            _manufacturer = HONDA;
            break;
        case 'S':
            _manufacturer = SUBARO;
            break;
        case 'T':
            _manufacturer = TESLA;
            break;
        case 'F':
            _manufacturer = FIAT;
            break;
        default:
            throw "TaxiFactory::setManufacturer: unknown manufacturer received";
    }
}
/**
 * Color setter
 * @param color -   'R': red
 *                  'G': green
 *                  'B': blue
 *                  'P': pink
 *                  'W': white
 *                  other char - throws exception.
 */
void TaxiFactory::setColor(char color) {
    switch(color){
        case 'R':
            _color = RED;
            break;
        case 'G':
            _color = GREEN;
            break;
        case 'B':
            _color = BLUE;
            break;
        case 'P':
            _color = PINK;
            break;
        case 'W':
            _color = WHITE;
            break;
        default:
            throw "TaxiFactory::setColor: unknown color received";
    }
}

/**
 * creates and returns a taxi by the setted properties.
 */
Taxi *TaxiFactory::createTaxi() {
    Taxi* returnedTaxi = NULL;
    switch(_type){
        case 1:
            returnedTaxi = new Taxi(Point(0,0), _id, 0, 1,_manufacturer, _color);
            break;
        case 2:
            //create a luxuryCab by setting the speed and tariff manually.
            returnedTaxi = new Taxi(Point(0,0), _id, 0, 2,_manufacturer, _color);
            returnedTaxi->setTariff(returnedTaxi->getTariff() * (float)2);
            break;
        default:
            return NULL;
    }
    return returnedTaxi;
}

